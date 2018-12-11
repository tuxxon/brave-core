/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ledger/public/cpp/ledger_client_mojo_proxy.h"

#include "mojo/public/cpp/bindings/map.h"

namespace bat_ledger {

namespace { // TODO, move into a util class

int32_t ToMojomResult(ledger::Result result) {
  return (int32_t)result;
}

ledger::Result ToLedgerResult(int32_t result) {
  return (ledger::Result)result;
}

ledger::URL_METHOD ToLedgerURLMethod(int32_t method) {
  return (ledger::URL_METHOD)method;
}

ledger::PUBLISHER_CATEGORY ToLedgerPublisherCategory(int32_t category) {
  return (ledger::PUBLISHER_CATEGORY)category;
}

ledger::Grant ToLedgerGrant(const std::string& grant_json) {
  ledger::Grant grant;
  grant.loadFromJson(grant_json);
  return grant;
}

} // anonymous namespace

LedgerClientMojoProxy::LedgerClientMojoProxy(
    ledger::LedgerClient* ledger_client)
  : ledger_client_(ledger_client) {
}

LedgerClientMojoProxy::~LedgerClientMojoProxy() {
}

void LedgerClientMojoProxy::LoadLedgerState(LoadLedgerStateCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  load_ledger_state_callback_ = std::move(callback);
  ledger_client_->LoadLedgerState(this);
}

void LedgerClientMojoProxy::OnLedgerStateLoaded(ledger::Result result,
                                                const std::string& data) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(load_ledger_state_callback_).Run(ToMojomResult(result), data);
}

void LedgerClientMojoProxy::GenerateGUID(GenerateGUIDCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(callback).Run(ledger_client_->GenerateGUID());
}

void LedgerClientMojoProxy::OnWalletInitialized(int32_t result) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  ledger_client_->OnWalletInitialized(ToLedgerResult(result));
}

void LedgerClientMojoProxy::LoadPublisherState(
    LoadPublisherStateCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  load_publisher_state_callback_ = std::move(callback);
  ledger_client_->LoadPublisherState(this);
}

void LedgerClientMojoProxy::OnPublisherStateLoaded(ledger::Result result,
                                                   const std::string& data) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(load_publisher_state_callback_).Run(ToMojomResult(result), data);
}

void LedgerClientMojoProxy::LoadPublisherList(
    LoadPublisherListCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  load_publisher_list_callback_ = std::move(callback);
  ledger_client_->LoadPublisherList(this);
}

void LedgerClientMojoProxy::OnPublisherListLoaded(ledger::Result result,
                                                  const std::string& data) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(load_publisher_list_callback_).Run(ToMojomResult(result), data);
}

void LedgerClientMojoProxy::SaveLedgerState(
    const std::string& ledger_state, SaveLedgerStateCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  save_ledger_state_callback_ = std::move(callback);
  ledger_client_->SaveLedgerState(ledger_state, this);
}

void LedgerClientMojoProxy::OnLedgerStateSaved(ledger::Result result) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(save_ledger_state_callback_).Run(ToMojomResult(result));
}

void LedgerClientMojoProxy::SavePublisherState(
    const std::string& publisher_state, SavePublisherStateCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  save_publisher_state_callback_ = std::move(callback);
  ledger_client_->SavePublisherState(publisher_state, this);
}

void LedgerClientMojoProxy::OnPublisherStateSaved(ledger::Result result) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(save_publisher_state_callback_).Run(ToMojomResult(result));
}

void LedgerClientMojoProxy::SavePublishersList(
    const std::string& publishers_list, SavePublishersListCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  save_publishers_list_callback_ = std::move(callback);
  ledger_client_->SavePublishersList(publishers_list, this);
}

void LedgerClientMojoProxy::OnPublishersListSaved(ledger::Result result) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  std::move(save_publishers_list_callback_).Run(ToMojomResult(result));
}

void LedgerClientMojoProxy::LoadURL(const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& content,
    const std::string& contentType,
    int32_t method,
    LoadURLCallback callback) {
  LOG(ERROR) << __PRETTY_FUNCTION__;
  load_url_callback_ = std::move(callback);
  ledger_client_->LoadURL(url, headers, content, contentType,
      ToLedgerURLMethod(method), this);
}

void LedgerClientMojoProxy::OnURLRequestResponse(uint64_t request_id,
      const std::string& url,
      int response_code,
      const std::string& response,
      const std::map<std::string, std::string>& headers) {
  std::move(load_url_callback_).Run(request_id, url, response_code, response,
      mojo::MapToFlatMap(headers));
}

void LedgerClientMojoProxy::OnGrant(int32_t result, const std::string& grant) {
  ledger_client_->OnGrant(ToLedgerResult(result), ToLedgerGrant(grant));
}

void LedgerClientMojoProxy::OnGrantCaptcha(const std::string& image,
    const std::string& hint) {
  ledger_client_->OnGrantCaptcha(image, hint);
}

void LedgerClientMojoProxy::OnRecoverWallet(int32_t result, double balance,
    const std::vector<std::string>& grants) {
  std::vector<ledger::Grant> ledger_grants;
  for (auto const& grant : grants) {
    ledger_grants.push_back(ToLedgerGrant(grant));
  }

  ledger_client_->OnRecoverWallet(
      ToLedgerResult(result), balance, ledger_grants);
}

void LedgerClientMojoProxy::OnReconcileComplete(int32_t result,
    const std::string& viewing_id,
    int32_t category,
    const std::string& probi) {
  ledger_client_->OnReconcileComplete(ToLedgerResult(result), viewing_id,
      ToLedgerPublisherCategory(category), probi);
}

void LedgerClientMojoProxy::OnGrantFinish(int32_t result,
    const std::string& grant) {
  ledger_client_->OnGrantFinish(ToLedgerResult(result), ToLedgerGrant(grant));
}

void LedgerClientMojoProxy::SetTimer(uint64_t time_offset,
    SetTimerCallback callback) {
  uint32_t timer_id;
  ledger_client_->SetTimer(time_offset, timer_id);
  std::move(callback).Run(timer_id);
}

void LedgerClientMojoProxy::OnExcludedSitesChanged() {
  ledger_client_->OnExcludedSitesChanged();
}

void LedgerClientMojoProxy::SaveContributionInfo(const std::string& probi,
    int32_t month, int32_t year, uint32_t date,
    const std::string& publisher_key, int32_t category) {
  ledger_client_->SaveContributionInfo(probi, month, year, date, publisher_key,
      ToLedgerPublisherCategory(category));
}

void LedgerClientMojoProxy::SaveMediaPublisherInfo(
    const std::string& media_key, const std::string& publisher_id) {
  ledger_client_->SaveMediaPublisherInfo(media_key, publisher_id);
}

void LedgerClientMojoProxy::FetchWalletProperties() {
  ledger_client_->FetchWalletProperties();
}

void LedgerClientMojoProxy::FetchGrant(const std::string& lang,
    const std::string& payment_id) {
  ledger_client_->FetchGrant(lang, payment_id);
}

void LedgerClientMojoProxy::GetGrantCaptcha() {
  ledger_client_->GetGrantCaptcha();
}

void LedgerClientMojoProxy::URIEncode(const std::string& value,
    URIEncodeCallback callback) {
  std::move(callback).Run(ledger_client_->URIEncode(value));
}

void LedgerClientMojoProxy::SetContributionAutoInclude(
    const std::string& publisher_key, bool excluded, uint64_t window_id) {
  ledger_client_->SetContributionAutoInclude(
      publisher_key, excluded, window_id);
}

} // namespace bat_ledger
