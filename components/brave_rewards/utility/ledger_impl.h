/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_REWARDS_UTILITY_LEDGER_IMPL_H_
#define BRAVE_COMPONENTS_BRAVE_REWARDS_UTILITY_LEDGER_IMPL_H_

#include <memory>

#include "bat/ledger/ledger.h"
#include "bat/ledger/ledger_client.h"
#include "brave/components/brave_rewards/common/ledger.mojom.h"
#include "services/service_manager/public/cpp/service_context_ref.h"

namespace rewards {

class LedgerImpl : public rewards::mojom::Ledger,
                   public ledger::LedgerClient {
 public:
  explicit LedgerImpl(
      std::unique_ptr<service_manager::ServiceContextRef> service_ref);
  ~LedgerImpl() override;

  // rewards::mojom::Ledger
  void Initialize() override;

 private:
  const std::unique_ptr<service_manager::ServiceContextRef> service_ref_;
  std::unique_ptr<ledger::Ledger> ledger_;

  DISALLOW_COPY_AND_ASSIGN(LedgerImpl);
};

} // namespace rewards

#endif // BRAVE_COMPONENTS_BRAVE_REWARDS_UTILITY_LEDGER_IMPL_H_
