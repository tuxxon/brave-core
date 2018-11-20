/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/utility/mojo_ledger_client.h"

namespace rewards {

MojoLedgerClient::MojoLedgerClient() {
}

MojoLedgerClient::~MojoLedgerClient() {
}

// static
MojoLedgerClient* MojoLedgerClient::Create() {
  return new MojoLedgerClient();
}

std::string MojoLedgerClient::GenerateGUID() const {
  return "";
}

std::unique_ptr<ledger::LedgerURLLoader> MojoLedgerClient::LoadURL(
    const std::string& url,
    const std::vector<std::string>& headers,
    const std::string& content,
    const std::string& contentType,
    const ledger::URL_METHOD& method,
    ledger::LedgerCallbackHandler* handler) {
  return nullptr;
}

std::string MojoLedgerClient::URIEncode(const std::string& value) {
  return "";
}

void MojoLedgerClient::OnExcludedSitesChanged() {
  ledger_client_->OnExcludedSitesChanged();
}

} // namespace rewards
