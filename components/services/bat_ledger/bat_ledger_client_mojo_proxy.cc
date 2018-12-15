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

void BatLedgerClientMojoProxy::LoadURL(
    const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& content,
    const std::string& contentType,
    const ledger::URL_METHOD& method,
    ledger::LoadURLCallback callback) {
}

std::string BatLedgerClientMojoProxy::URIEncode(const std::string& value) {
  return "";
}

void BatLedgerClientMojoProxy::OnExcludedSitesChanged(const std::string& publisher_id) {
//  bat_ledger_client_->OnExcludedSitesChanged();
}


std::unique_ptr<ledger::LogStream> BatLedgerClientMojoProxy::Log(
    const char* file, int line, ledger::LogLevel level) const {
  return nullptr;
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

} // namespace bat_ledger
