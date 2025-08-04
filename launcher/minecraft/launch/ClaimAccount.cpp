#include "ClaimAccount.h"
#include <launch/LaunchTask.h>

#include "Application.h"
#include "minecraft/auth/AccountList.h"

ClaimAccount::ClaimAccount(LaunchTask* parent, AuthSessionPtr session) : LaunchStep(parent)
{
    if (session->status == AuthSession::Status::PlayableOnline && !session->demo) {
        auto accounts = APPLICATION->accounts();
        m_account = accounts->getAccountByProfileName(session->player_name);
    }
}

void ClaimAccount::executeTask()
{
    if (m_account) {
        m_lock = std::make_unique<MinecraftAccountLock>(m_account);
        emitSucceeded();
    }
}

void ClaimAccount::finalize()
{
    m_lock.reset();
}
