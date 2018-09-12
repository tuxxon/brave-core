/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/extensions/api/brave_shields_api.h"

#include "brave/common/extensions/api/brave_shields.h"
#include "brave/common/extensions/extension_constants.h"
#include "brave/components/brave_shields/browser/brave_shields_web_contents_observer.h"
#include "chrome/browser/content_settings/host_content_settings_map_factory.h"
#include "chrome/browser/extensions/api/content_settings/content_settings_api_constants.h"
#include "chrome/browser/extensions/api/content_settings/content_settings_helpers.h"
#include "chrome/browser/extensions/api/content_settings/content_settings_service.h"
#include "chrome/browser/extensions/api/content_settings/content_settings_store.h"
#include "chrome/browser/extensions/api/tabs/tabs_constants.h"
#include "chrome/browser/extensions/extension_tab_util.h"
#include "chrome/browser/profiles/profile.h"
#include "components/content_settings/core/browser/content_settings_registry.h"
#include "components/content_settings/core/browser/content_settings_utils.h"
#include "components/content_settings/core/browser/host_content_settings_map.h"
#include "content/public/browser/web_contents.h"

using brave_shields::BraveShieldsWebContentsObserver;

namespace Get = extensions::api::brave_shields::ContentSetting::Get;
namespace Set = extensions::api::brave_shields::ContentSetting::Set;

namespace {

bool RemoveContentType(base::ListValue* args,
                       ContentSettingsType* content_type) {
  std::string content_type_str;
  if (!args->GetString(0, &content_type_str))
    return false;
  // We remove the ContentSettingsType parameter since this is added by the
  // renderer, and is not part of the JSON schema.
  args->Remove(0, nullptr);
  *content_type =
      extensions::content_settings_helpers::StringToContentSettingsType(
          content_type_str);
  return *content_type != CONTENT_SETTINGS_TYPE_DEFAULT;
}

}  // namespace

namespace extensions {
namespace api {

BraveShieldsAllowScriptsOnceFunction::~BraveShieldsAllowScriptsOnceFunction() {
}

ExtensionFunction::ResponseAction BraveShieldsAllowScriptsOnceFunction::Run() {
  std::unique_ptr<brave_shields::AllowScriptsOnce::Params> params(
      brave_shields::AllowScriptsOnce::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  // Get web contents for this tab
  content::WebContents* contents = nullptr;
  if (!ExtensionTabUtil::GetTabById(
        params->tab_id,
        Profile::FromBrowserContext(browser_context()),
        include_incognito_information(),
        nullptr,
        nullptr,
        &contents,
        nullptr)) {
    return RespondNow(Error(tabs_constants::kTabNotFoundError,
                            base::IntToString(params->tab_id)));
  }

  BraveShieldsWebContentsObserver::FromWebContents(
      contents)->AllowScriptsOnce(params->origins, contents);
  return RespondNow(NoArguments());
}

ExtensionFunction::ResponseAction
BraveShieldsContentSettingGetFunction::Run() {
  ContentSettingsType content_type;
  EXTENSION_FUNCTION_VALIDATE(RemoveContentType(args_.get(), &content_type));

  std::unique_ptr<Get::Params> params(Get::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  GURL primary_url(params->details.primary_url);
  if (!primary_url.is_valid()) {
    return RespondNow(Error(content_settings_api_constants::kInvalidUrlError,
                            params->details.primary_url));
  }

  GURL secondary_url(primary_url);
  if (params->details.secondary_url.get()) {
    secondary_url = GURL(*params->details.secondary_url);
    if (!secondary_url.is_valid()) {
      return RespondNow(Error(content_settings_api_constants::kInvalidUrlError,
                              *params->details.secondary_url));
    }
  }

  std::string resource_identifier;
  if (params->details.resource_identifier.get())
    resource_identifier = params->details.resource_identifier->id;

  Profile* profile = Profile::FromBrowserContext(browser_context());
  HostContentSettingsMap* map =
      HostContentSettingsMapFactory::GetForProfile(profile);

  ContentSetting setting = map->GetContentSetting(
      primary_url, secondary_url, content_type, resource_identifier);

  std::unique_ptr<base::DictionaryValue> result(new base::DictionaryValue());
  std::string setting_string =
      content_settings::ContentSettingToString(setting);
  DCHECK(!setting_string.empty());
  result->SetString(content_settings_api_constants::kContentSettingKey,
                    setting_string);

  return RespondNow(OneArgument(std::move(result)));
}

ExtensionFunction::ResponseAction
BraveShieldsContentSettingSetFunction::Run() {
  ContentSettingsType content_type;
  EXTENSION_FUNCTION_VALIDATE(RemoveContentType(args_.get(), &content_type));

  std::unique_ptr<Set::Params> params(Set::Params::Create(*args_));
  EXTENSION_FUNCTION_VALIDATE(params.get());

  std::string resource_identifier;
  if (params->details.resource_identifier.get())
    resource_identifier = params->details.resource_identifier->id;

  std::string setting_str;
  EXTENSION_FUNCTION_VALIDATE(
      params->details.setting->GetAsString(&setting_str));
  ContentSetting setting;
  EXTENSION_FUNCTION_VALIDATE(
      content_settings::ContentSettingFromString(setting_str, &setting));
  EXTENSION_FUNCTION_VALIDATE(CONTENT_SETTING_DEFAULT != setting);
  EXTENSION_FUNCTION_VALIDATE(
      content_settings::ContentSettingsRegistry::GetInstance()
          ->Get(content_type)
          ->IsSettingValid(setting));

  std::string primary_error;
  ContentSettingsPattern primary_pattern =
      content_settings_helpers::ParseExtensionPattern(
          params->details.primary_pattern, &primary_error);
  if (!primary_pattern.IsValid())
    return RespondNow(Error(primary_error));
  ContentSettingsPattern secondary_pattern =
      ContentSettingsPattern::Wildcard();
  if (params->details.secondary_pattern.get()) {
    std::string secondary_error;
    secondary_pattern = content_settings_helpers::ParseExtensionPattern(
        *params->details.secondary_pattern, &secondary_error);
    if (!secondary_pattern.IsValid())
      return RespondNow(Error(secondary_error));
  }

  Profile* profile = Profile::FromBrowserContext(browser_context());
  HostContentSettingsMap* map =
      HostContentSettingsMapFactory::GetForProfile(profile);
  if (content_type == CONTENT_SETTINGS_TYPE_JAVASCRIPT) {
    // TODO(simonhong): Need to check why generating pattern with
    // content_settings_helpers::ParseExtensionPattern() causes javascript
    // set fail.
    const GURL primary_url(params->details.primary_pattern);
    if (!primary_url.is_valid())
      return RespondNow(Error("Invalid url"));

    map->SetContentSettingDefaultScope(
        primary_url, primary_url, content_type, resource_identifier, setting);
  } else {
    map->SetContentSettingCustomScope(
        primary_pattern, secondary_pattern,
        content_type, resource_identifier, setting);
  }

  // Delete previous settings set by brave extension in extension's
  // ContentSettingsStore. Setting default means delete.
  scoped_refptr<ContentSettingsStore> store =
      ContentSettingsService::Get(browser_context())->content_settings_store();
  store->SetExtensionContentSetting(brave_extension_id,
                                    primary_pattern, secondary_pattern,
                                    content_type,
                                    resource_identifier,
                                    CONTENT_SETTING_DEFAULT,
                                    kExtensionPrefsScopeRegular);
  return RespondNow(NoArguments());
}

}  // namespace api
}  // namespace extensions
