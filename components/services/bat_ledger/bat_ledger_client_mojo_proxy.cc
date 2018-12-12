/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ledger/bat_ledger_client_mojo_proxy.h"

#include "mojo/public/cpp/bindings/map.h"

namespace bat_ledger {

namespace {

int32_t ToMojomResult(ledger::Result result) {
  return (int32_t)result;
}

ledger::Result ToLedgerResult(int32_t result) {
  return (ledger::Result)result;
}

int32_t ToMojomPublisherCategory(ledger::PUBLISHER_CATEGORY category) {
  return (int32_t)category;
}

} // anonymous namespace

BatLedgerClientMojoProxy::BatLedgerClientMojoProxy(
    mojom::BatLedgerClientAssociatedPtrInfo client_info) {
  bat_ledger_client_.Bind(std::move(client_info));
}

BatLedgerClientMojoProxy::~BatLedgerClientMojoProxy() {
}

std::string BatLedgerClientMojoProxy::GenerateGUID() const {
  std::string guid;
  bat_ledger_client_->GenerateGUID(&guid);
  return guid;
}

void BatLedgerClientMojoProxy::OnWalletInitialized(ledger::Result result) {
  bat_ledger_client_->OnWalletInitialized(ToMojomResult(result));
}

void BatLedgerClientMojoProxy::OnWalletProperties(ledger::Result result,
    std::unique_ptr<ledger::WalletInfo> info) {
  bat_ledger_client_->OnWalletProperties(ToMojomResult(result),
      info->ToJson());
}

void BatLedgerClientMojoProxy::OnGrant(ledger::Result result,
    const ledger::Grant& grant) {
  bat_ledger_client_->OnGrant(ToMojomResult(result), grant.ToJson());
}

void BatLedgerClientMojoProxy::OnGrantCaptcha(const std::string& image,
    const std::string& hint) {
  bat_ledger_client_->OnGrantCaptcha(image, hint);
}

void BatLedgerClientMojoProxy::OnRecoverWallet(ledger::Result result,
    double balance, const std::vector<ledger::Grant>& grants) {
  std::vector<std::string> grant_jsons;
  for (auto const& grant : grants) {
    grant_jsons.push_back(grant.ToJson());
  }

  bat_ledger_client_->OnRecoverWallet(
      ToMojomResult(result), balance, grant_jsons);
}

void BatLedgerClientMojoProxy::OnReconcileComplete(ledger::Result result,
    const std::string& viewing_id,
    ledger::PUBLISHER_CATEGORY category,
    const std::string& probi) {
  bat_ledger_client_->OnReconcileComplete(ToMojomResult(result), viewing_id,
      ToMojomPublisherCategory(category), probi);
}

void BatLedgerClientMojoProxy::OnGrantFinish(ledger::Result result,
    const ledger::Grant& grant) {
  bat_ledger_client_->OnGrantFinish(ToMojomResult(result), grant.ToJson());
}

void BatLedgerClientMojoProxy::OnLoadLedgerState(ledger::LedgerCallbackHandler* handler,
    int32_t result, const std::string& data) {
  handler->OnLedgerStateLoaded(ToLedgerResult(result), data);
}

void BatLedgerClientMojoProxy::LoadLedgerState(
    ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->LoadLedgerState(
      base::BindOnce(&BatLedgerClientMojoProxy::OnLoadLedgerState, AsWeakPtr(),
        base::Unretained(handler)));
}

void BatLedgerClientMojoProxy::OnLoadPublisherState(
    ledger::LedgerCallbackHandler* handler,
    int32_t result, const std::string& data) {
  handler->OnPublisherStateLoaded(ToLedgerResult(result), data);
}

void BatLedgerClientMojoProxy::LoadPublisherState(
    ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->LoadPublisherState(
      base::BindOnce(&BatLedgerClientMojoProxy::OnLoadPublisherState,
        AsWeakPtr(), base::Unretained(handler)));
}

void BatLedgerClientMojoProxy::OnLoadPublisherList(
    ledger::LedgerCallbackHandler* handler,
    int32_t result, const std::string& data) {
  handler->OnPublisherListLoaded(ToLedgerResult(result), data);
}

void BatLedgerClientMojoProxy::LoadPublisherList(
    ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->LoadPublisherList(
      base::BindOnce(&BatLedgerClientMojoProxy::OnLoadPublisherList,
        AsWeakPtr(), base::Unretained(handler)));
}

void BatLedgerClientMojoProxy::OnSaveLedgerState(
    ledger::LedgerCallbackHandler* handler,
    int32_t result) {
  handler->OnLedgerStateSaved(ToLedgerResult(result));
}

void BatLedgerClientMojoProxy::SaveLedgerState(
    const std::string& ledger_state, ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->SaveLedgerState(ledger_state,
      base::BindOnce(&BatLedgerClientMojoProxy::OnSaveLedgerState,
        AsWeakPtr(), base::Unretained(handler)));
}

void BatLedgerClientMojoProxy::OnSavePublisherState(
    ledger::LedgerCallbackHandler* handler,
    int32_t result) {
  handler->OnPublisherStateSaved(ToLedgerResult(result));
}

void BatLedgerClientMojoProxy::SavePublisherState(
    const std::string& publisher_state,
    ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->SavePublisherState(publisher_state,
      base::BindOnce(&BatLedgerClientMojoProxy::OnSavePublisherState,
        AsWeakPtr(), base::Unretained(handler)));
}

void BatLedgerClientMojoProxy::OnSavePublishersList(
    ledger::LedgerCallbackHandler* handler,
    int32_t result) {
  handler->OnPublishersListSaved(ToLedgerResult(result));
}

void BatLedgerClientMojoProxy::SavePublishersList(
    const std::string& publishers_list,
    ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->SavePublishersList(publishers_list,
      base::BindOnce(&BatLedgerClientMojoProxy::OnSavePublishersList,
        AsWeakPtr(), base::Unretained(handler)));
}

void BatLedgerClientMojoProxy::OnLoadURL(
    ledger::LedgerCallbackHandler* handler,
    uint64_t request_id, const std::string& url,
    int32_t response_code, const std::string& response,
    const base::flat_map<std::string, std::string>& headers) {
  handler->OnURLRequestResponse(request_id, url, response_code, response,
      mojo::FlatMapToMap(headers));
}

std::unique_ptr<ledger::LedgerURLLoader> BatLedgerClientMojoProxy::LoadURL(
    const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& content,
    const std::string& contentType,
    const ledger::URL_METHOD& method,
    ledger::LedgerCallbackHandler* handler) {
  bat_ledger_client_->LoadURL(url, headers, content, contentType, method,
      base::BindOnce(&BatLedgerClientMojoProxy::OnLoadURL,
        AsWeakPtr(), base::Unretained(handler)));
  return nullptr; // TODO
}

void BatLedgerClientMojoProxy::SetTimer(uint64_t time_offset,
    uint32_t& timer_id) {
  bat_ledger_client_->SetTimer(time_offset, &timer_id); // sync
}

void BatLedgerClientMojoProxy::OnExcludedSitesChanged() {
  bat_ledger_client_->OnExcludedSitesChanged();
}

void BatLedgerClientMojoProxy::OnPublisherActivity(ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> info,
    uint64_t windowId) {
  bat_ledger_client_->OnPublisherActivity(ToMojomResult(result),
      info->ToJson(), windowId);
}

void BatLedgerClientMojoProxy::SaveContributionInfo(const std::string& probi,
    const int month,
    const int year,
    const uint32_t date,
    const std::string& publisher_key,
    const ledger::PUBLISHER_CATEGORY category) {
  bat_ledger_client_->SaveContributionInfo(probi, month, year, date,
      publisher_key, ToMojomPublisherCategory(category));
}

void BatLedgerClientMojoProxy::SaveMediaPublisherInfo(
    const std::string& media_key, const std::string& publisher_id) {
  bat_ledger_client_->SaveMediaPublisherInfo(media_key, publisher_id);
}

void BatLedgerClientMojoProxy::FetchWalletProperties() {
  bat_ledger_client_->FetchWalletProperties();
}

void BatLedgerClientMojoProxy::FetchGrant(const std::string& lang,
    const std::string& paymentId) {
  bat_ledger_client_->FetchGrant(lang, paymentId);
}

void BatLedgerClientMojoProxy::GetGrantCaptcha() {
  bat_ledger_client_->GetGrantCaptcha();
}

std::string BatLedgerClientMojoProxy::URIEncode(const std::string& value) {
  std::string encoded_value;
  bat_ledger_client_->URIEncode(value, &encoded_value);
  return encoded_value;
}

void BatLedgerClientMojoProxy::SetContributionAutoInclude(
  const std::string& publisher_key, bool excluded, uint64_t windowId) {
  bat_ledger_client_->SetContributionAutoInclude(
      publisher_key, excluded, windowId);
}

} // namespace bat_ledger
