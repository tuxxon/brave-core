/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/services/bat_ledger/public/cpp/ledger_client_mojo_proxy.h"

namespace bat_ledger {

namespace {

int32_t ToMojomResult(ledger::Result result) {
  return (int32_t)result;
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

} // namespace bat_ledger
