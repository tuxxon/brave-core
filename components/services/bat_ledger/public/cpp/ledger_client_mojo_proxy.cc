/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ledger/public/cpp/ledger_client_mojo_proxy.h"

#include "mojo/public/cpp/bindings/map.h"

using namespace std::placeholders;

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

void LedgerClientMojoProxy::OnWalletProperties(int32_t result,
    const std::string& info) {
  std::unique_ptr<ledger::WalletInfo> wallet_info;
  wallet_info->loadFromJson(info);
  ledger_client_->OnWalletProperties(ToLedgerResult(result),
      std::move(wallet_info));
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

// static
void LedgerClientMojoProxy::OnSavePublisherInfo(
    CallbackHolder<SavePublisherInfoCallback>* holder,
    ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> info) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result), info->ToJson());
  delete holder;
}

void LedgerClientMojoProxy::SavePublisherInfo(
    const std::string& publisher_info,
    SavePublisherInfoCallback callback) {
  // deleted in OnSavePublisherInfo
  auto* holder = new CallbackHolder<SavePublisherInfoCallback>(
      AsWeakPtr(), std::move(callback));
  std::unique_ptr<ledger::PublisherInfo> info;
  info->loadFromJson(publisher_info);
  ledger_client_->SavePublisherInfo(std::move(info),
      std::bind(LedgerClientMojoProxy::OnSavePublisherInfo, holder, _1, _2));
}

// static
void LedgerClientMojoProxy::OnLoadPublisherInfo(
    CallbackHolder<LoadPublisherInfoCallback>* holder,
    ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> info) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result), info->ToJson());
  delete holder;
}

void LedgerClientMojoProxy::LoadPublisherInfo(
    const std::string& filter,
    LoadPublisherInfoCallback callback) {
  // deleted in OnLoadPublisherInfo
  auto* holder = new CallbackHolder<LoadPublisherInfoCallback>(
      AsWeakPtr(), std::move(callback));
  ledger::PublisherInfoFilter publisher_info_filter;
  publisher_info_filter.loadFromJson(filter);
  ledger_client_->LoadPublisherInfo(publisher_info_filter,
      std::bind(LedgerClientMojoProxy::OnLoadPublisherInfo, holder, _1, _2));
}

// static
void LedgerClientMojoProxy::OnLoadPublisherInfoList(
    CallbackHolder<LoadPublisherInfoListCallback>* holder,
    const ledger::PublisherInfoList& list,
    uint32_t next_record) {
  std::vector<std::string> publisher_info_list;
  for (const auto& info : list) {
    publisher_info_list.push_back(info.ToJson());
  }

  if (holder->is_valid())
    std::move(holder->get()).Run(publisher_info_list, next_record);
  delete holder;
}

void LedgerClientMojoProxy::LoadPublisherInfoList(uint32_t start,
    uint32_t limit,
    const std::string& filter,
    LoadPublisherInfoListCallback callback) {
  // deleted in OnLoadPublisherInfoList
  auto* holder = new CallbackHolder<LoadPublisherInfoListCallback>(
      AsWeakPtr(), std::move(callback));
  ledger::PublisherInfoFilter publisher_info_filter;
  publisher_info_filter.loadFromJson(filter);
  ledger_client_->LoadPublisherInfoList(start, limit, publisher_info_filter,
      std::bind(LedgerClientMojoProxy::OnLoadPublisherInfoList,
        holder, _1, _2));
}

// static
void LedgerClientMojoProxy::OnLoadCurrentPublisherInfoList(
    CallbackHolder<LoadCurrentPublisherInfoListCallback>* holder,
    const ledger::PublisherInfoList& list,
    uint32_t next_record) {
  std::vector<std::string> publisher_info_list;
  for (const auto& info : list) {
    publisher_info_list.push_back(info.ToJson());
  }

  if (holder->is_valid())
    std::move(holder->get()).Run(publisher_info_list, next_record);
  delete holder;
}

void LedgerClientMojoProxy::LoadCurrentPublisherInfoList(uint32_t start,
    uint32_t limit,
    const std::string& filter,
    LoadCurrentPublisherInfoListCallback callback) {
  // deleted in OnLoadCurrentPublisherInfoList
  auto* holder = new CallbackHolder<LoadCurrentPublisherInfoListCallback>(
      AsWeakPtr(), std::move(callback));
  ledger::PublisherInfoFilter publisher_info_filter;
  publisher_info_filter.loadFromJson(filter);
  ledger_client_->LoadCurrentPublisherInfoList(start, limit,
      publisher_info_filter,
      std::bind(LedgerClientMojoProxy::OnLoadCurrentPublisherInfoList,
        holder, _1, _2));
}

// static
void LedgerClientMojoProxy::OnLoadMediaPublisherInfo(
    CallbackHolder<LoadMediaPublisherInfoCallback>* holder,
    ledger::Result result,
    std::unique_ptr<ledger::PublisherInfo> info) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToMojomResult(result), info->ToJson());
  delete holder;
}

void LedgerClientMojoProxy::LoadMediaPublisherInfo(
    const std::string& media_key,
    LoadMediaPublisherInfoCallback callback) {
  // deleted in OnLoadMediaPublisherInfo
  auto* holder = new CallbackHolder<LoadMediaPublisherInfoCallback>(
      AsWeakPtr(), std::move(callback));
  ledger_client_->LoadMediaPublisherInfo(media_key,
      std::bind(LedgerClientMojoProxy::OnLoadMediaPublisherInfo, holder, _1, _2));
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

void LedgerClientMojoProxy::OnPublisherActivity(int32_t result,
    const std::string& info, uint64_t window_id) {
  std::unique_ptr<ledger::PublisherInfo> publisher_info;
  publisher_info->loadFromJson(info);
  ledger_client_->OnPublisherActivity(ToLedgerResult(result),
      std::move(publisher_info), window_id);
}

// static
void LedgerClientMojoProxy::OnFetchFavIcon(
    CallbackHolder<FetchFavIconCallback>* holder,
    bool success, const std::string& favicon_url) {
  if (holder->is_valid())
    std::move(holder->get()).Run(success, favicon_url);
  delete holder;
}

void LedgerClientMojoProxy::FetchFavIcon(const std::string& url,
    const std::string& favicon_key, FetchFavIconCallback callback) {
  // deleted in OnFetchFavIcon
  auto* holder = new CallbackHolder<FetchFavIconCallback>(
      AsWeakPtr(), std::move(callback));
  ledger_client_->FetchFavIcon(url, favicon_key,
      std::bind(LedgerClientMojoProxy::OnFetchFavIcon, holder, _1, _2));

}

// static
void LedgerClientMojoProxy::OnGetRecurringDonations(
    CallbackHolder<GetRecurringDonationsCallback>* holder,
    const ledger::PublisherInfoList& publisher_info_list,
    uint32_t next_record) {
  std::vector<std::string> list;
  for (const auto& publisher_info : publisher_info_list) {
    list.push_back(publisher_info.ToJson());
  }

  if (holder->is_valid())
    std::move(holder->get()).Run(list, next_record);
  delete holder;
}

void LedgerClientMojoProxy::GetRecurringDonations(
    GetRecurringDonationsCallback callback) {
  // deleted in OnGetRecurringDonations
  auto* holder = new CallbackHolder<GetRecurringDonationsCallback>(
      AsWeakPtr(), std::move(callback));
  ledger_client_->GetRecurringDonations(
      std::bind(LedgerClientMojoProxy::OnGetRecurringDonations,
        holder, _1, _2));
}

// static
void LedgerClientMojoProxy::OnLoadNicewareList(
    CallbackHolder<LoadNicewareListCallback>* holder,
    int32_t result, const std::string& data) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToLedgerResult(result), data);
  delete holder;
}

void LedgerClientMojoProxy::LoadNicewareList(
    LoadNicewareListCallback callback) {
  // deleted in OnLoadNicewareList
  auto* holder = new CallbackHolder<LoadNicewareListCallback>(
      AsWeakPtr(), std::move(callback));
  ledger_client_->LoadNicewareList(
      std::bind(LedgerClientMojoProxy::OnLoadNicewareList,
        holder, _1, _2));
}

// static
void LedgerClientMojoProxy::OnRecurringRemoved(
    CallbackHolder<OnRemoveRecurringCallback>* holder, int32_t result) {
  if (holder->is_valid())
    std::move(holder->get()).Run(ToLedgerResult(result));
  delete holder;
}

void LedgerClientMojoProxy::OnRemoveRecurring(const std::string& publisher_key,
    OnRemoveRecurringCallback callback) {
  // deleted in OnRecurringRemoved
  auto* holder = new CallbackHolder<OnRemoveRecurringCallback>(
      AsWeakPtr(), std::move(callback));
  ledger_client_->OnRemoveRecurring(publisher_key,
      std::bind(LedgerClientMojoProxy::OnRecurringRemoved, holder, _1));
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
