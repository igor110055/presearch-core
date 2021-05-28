/* Copyright (c) 2020 The Presearch Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef PRESEARCHLEDGER_DATABASE_MIGRATION_MIGRATION_V12_H_
#define PRESEARCHLEDGER_DATABASE_MIGRATION_MIGRATION_V12_H_

namespace ledger {
namespace database {
namespace migration {

const char v12[] = R"(
  ALTER TABLE pending_contribution RENAME TO pending_contribution_temp;

  DROP INDEX IF EXISTS pending_contribution_publisher_id_index;

  CREATE TABLE pending_contribution (
    pending_contribution_id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
    publisher_id LONGVARCHAR NOT NULL,
    amount DOUBLE DEFAULT 0 NOT NULL,
    added_date INTEGER DEFAULT 0 NOT NULL,
    viewing_id LONGVARCHAR NOT NULL,
    type INTEGER NOT NULL,
    CONSTRAINT fk_pending_contribution_publisher_id
      FOREIGN KEY (publisher_id)
      REFERENCES publisher_info (publisher_id)
      ON DELETE CASCADE
  );

  CREATE INDEX pending_contribution_publisher_id_index
    ON pending_contribution (publisher_id);

  PRAGMA foreign_keys = off;
    INSERT INTO pending_contribution (added_date, amount, publisher_id, type,
    viewing_id) SELECT added_date, amount, publisher_id, type, viewing_id
    FROM pending_contribution_temp;

    DROP TABLE pending_contribution_temp;
  PRAGMA foreign_keys = on;
)";

}  // namespace migration
}  // namespace database
}  // namespace ledger

#endif  // PRESEARCHLEDGER_DATABASE_MIGRATION_MIGRATION_V12_H_
