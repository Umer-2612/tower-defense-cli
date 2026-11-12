//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "pooler.h"

Pooler::Pooler(size_t minimumPoolSize) : minimumPoolSize(minimumPoolSize) {
    for (size_t i = 0; i < minimumPoolSize; ++i) {
        Enemy* enemy = CreateEnemy();
        enemies.push_back(enemy);
        freeList.push_back(enemy);
    }
}

Pooler::~Pooler() {
    for (Enemy* enemy : enemies) {
        delete enemy;
    }
}

size_t Pooler::PoolSize()
{
    return enemies.size();
}

Enemy* Pooler::SpawnEnemy(const Vector2D& spawnPosition) {
    Enemy* enemy;

    if (!freeList.empty()) {
        enemy = freeList.back();
        freeList.pop_back();
    } else {
        enemy = CreateEnemy();
        enemies.push_back(enemy);
    }

    enemy->Activate(spawnPosition);
    activeEnemiesCacheDirty = true;

    return enemy;
}

void Pooler::DespawnEnemy(Enemy* enemy) {
    enemy->Deactivate();
    freeList.push_back(enemy);
    activeEnemiesCacheDirty = true;
}

const std::vector<Enemy*>& Pooler::GetActiveEnemies() const {
    if (activeEnemiesCacheDirty) {
        activeEnemiesCache.clear();

        for (Enemy* enemy : enemies) {
            if (enemy->IsActive()) {
                activeEnemiesCache.push_back(enemy);
            }
        }

        activeEnemiesCacheDirty = false;
    }

    return activeEnemiesCache;
}

void Pooler::Clear() {
    for (Enemy* enemy : enemies) {
        if (enemy->IsActive()) {
            enemy->Deactivate();
            freeList.push_back(enemy);
        }
    }

    // Everything is inactive at this point, so enemies and freeList hold the
    // same set of pointers. Trimming has to remove from both, otherwise
    // freeList could hand back a pointer to something already deleted.
    while (enemies.size() > minimumPoolSize && !freeList.empty()) {
        Enemy* enemy = freeList.back();
        freeList.pop_back();

        enemies.remove(enemy);
        delete enemy;
    }

    activeEnemiesCacheDirty = true;
}

Enemy* Pooler::CreateEnemy() {
    return new Enemy();
}
