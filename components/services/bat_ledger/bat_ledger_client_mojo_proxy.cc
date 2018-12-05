/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ledger/bat_ledger_client_mojo_proxy.h"

namespace bat_ledger {

namespace {

ledger::Result ToLedgerResult(int32_t result) {
  return (ledger::Result)result;
}

} // anonymous namespace

BatLedgerClientMojoProxy::BatLedgerClientMojoProxy(
    mojom::BatLedgerClientAssociatedPtrInfo client_info) {
  bat_ledger_client_.Bind(std::move(client_info));
}

BatLedgerClientMojoProxy::~BatLedgerClientMojoProxy() {
}

std::string BatLedgerClientMojoProxy::GenerateGUID() const {
  return "";
}

std::unique_ptr<ledger::LedgerURLLoader> BatLedgerClientMojoProxy::LoadURL(
    const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& content,
    const std::string& contentType,
    const ledger::URL_METHOD& method,
    ledger::LedgerCallbackHandler* handler) {
  return nullptr;
}

std::string BatLedgerClientMojoProxy::URIEncode(const std::string& value) {
  return "";
}

void BatLedgerClientMojoProxy::OnExcludedSitesChanged() {
//  bat_ledger_client_->OnExcludedSitesChanged();
}

void BatLedgerClientMojoProxy::Test() {
  bat_ledger_client_->Test();
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

} // namespace bat_ledger
