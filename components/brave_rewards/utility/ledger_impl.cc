/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_rewards/utility/ledger_impl.h"

#include <utility>

namespace rewards {

LedgerImpl::LedgerImpl(
    std::unique_ptr<service_manager::ServiceContextRef> service_ref)
  : service_ref_(std::move(service_ref)),
    ledger_(ledger::Ledger::CreateInstance(this)) {

  // Environment
  #if defined(OFFICIAL_BUILD)
    ledger::is_production = true;
  #else
    ledger::is_production = false;
  #endif

}

LedgerImpl::~LedgerImpl() {
}

void LedgerImpl::Initialize() {
  LOG(ERROR) << __FUNCTION__;
  // ledger_->Initialize();
}

} // namespace rewards
