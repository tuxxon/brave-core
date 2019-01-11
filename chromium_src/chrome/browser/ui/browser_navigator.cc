/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/browser/renderer_host/brave_navigation_ui_data.h"

#define ChromeNavigationUIData BraveNavigationUIData
#define Navigate Navigate_ChromiumImpl
#define IsURLAllowedInIncognito IsURLAllowedInIncognito_ChromiumImpl
#include "../../../../chrome/browser/ui/browser_navigator.cc"
#undef ChromeNavigationUIData
#undef Navigate
#undef IsURLAllowedInIncognito

void Navigate(NavigateParams* params) {
  GURL& url = params->url;
  if (url.is_valid() && url.SchemeIs(content::kBraveUIScheme)) {
    GURL::Replacements replacements;
    replacements.SetSchemeStr(content::kChromeUIScheme);
    url = url.ReplaceComponents(replacements);
  }

  Navigate_ChromiumImpl(params);
}

bool IsURLAllowedInIncognito(const GURL& url,
                             content::BrowserContext* browser_context) {
  DCHECK(!url.SchemeIs(content::kBraveUIScheme))
      << "brave url should not be reached here. scheme mapping should be done "
         "before. URL: "
      << url.spec();

  return IsURLAllowedInIncognito_ChromiumImpl(url, browser_context);
}
