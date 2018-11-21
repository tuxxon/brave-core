/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/utility/brave_content_utility_client.h"

#include "brave/common/tor/tor_launcher.mojom.h"
#include "brave/components/brave_rewards/common/ledger.mojom.h"
#include "brave/components/brave_rewards/utility/ledger_service.h"
#include "brave/utility/tor/tor_launcher_service.h"

BraveContentUtilityClient::BraveContentUtilityClient()
    : ChromeContentUtilityClient() {}

BraveContentUtilityClient::~BraveContentUtilityClient() = default;

void BraveContentUtilityClient::RegisterServices(
    ChromeContentUtilityClient::StaticServiceMap* services) {
  ChromeContentUtilityClient::RegisterServices(services);
  LOG(ERROR) << __FUNCTION__;
  service_manager::EmbeddedServiceInfo tor_launcher_info;
  tor_launcher_info.factory = base::BindRepeating(
    &tor::TorLauncherService::CreateService);
  services->emplace(tor::mojom::kTorLauncherServiceName, tor_launcher_info);

  service_manager::EmbeddedServiceInfo ledger_info;
  ledger_info.factory = base::BindRepeating(
    &rewards::LedgerService::CreateService);
  services->emplace(rewards::mojom::kLedgerServiceName, ledger_info);
}
