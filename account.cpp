#include "otpch.h"
#include "account.h"
#include "database.h"
#include "databasetasks.h"

uint32_t IOAccount::getCoinBalance(uint32_t accountId) {
    std::ostringstream query;
    query << "SELECT `premium_points` FROM `accounts` WHERE `id` = " << accountId;

    DBResult_ptr result = Database::getInstance()->storeQuery(query.str());
    if (!result) {
        return false;
    }

    return result->getNumber<uint32_t>("premium_points");
}

void IOAccount::addCoins(uint32_t accountId, int32_t amount) {
    std::ostringstream query;
    query << "UPDATE `accounts` SET `premium_points` = `premium_points` + " << amount << " WHERE `id` = " << accountId;

    g_databaseTasks.addTask(query.str());
}

void IOAccount::removeCoins(uint32_t accountId, int32_t amount) {
    std::ostringstream query;
    query << "UPDATE `accounts` SET `premium_points` = `premium_points` - " << amount << " WHERE `id` = " << accountId;

    g_databaseTasks.addTask(query.str());
}

void IOAccount::registerTransaction(uint32_t accountId, int32_t coins, const std::string& description){
    Database* db = Database::getInstance();

    std::ostringstream query;
    query << "INSERT INTO `store_history` (`account_id`, `coin_amount`, `description`, `time`) VALUES (" << accountId << "," << coins << "," << db->escapeString(description) << "," << time(nullptr) << ")";

    db->executeQuery(query.str());
}
