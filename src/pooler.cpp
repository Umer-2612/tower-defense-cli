//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "pooler.h"

#include <algorithm>

Pooler::Pooler(size_t minimumPoolSize) : minimumPoolSize(minimumPoolSize) {
    for (size_t i = 0; i < minimumPoolSize; ++i) {
        enemies.push_back(CreateEnemy());
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
    auto inactiveEnemy = std::find_if(enemies.begin(), enemies.end(), [](Enemy* enemy) {
        return !enemy->IsActive();
    });

    Enemy* enemy;
    if (inactiveEnemy != enemies.end()) {
        enemy = *inactiveEnemy;
    } else {
        enemy = CreateEnemy();
        enemies.push_back(enemy);
    }

    enemy->Activate(spawnPosition);
    return enemy;
}

void Pooler::DespawnEnemy(Enemy* enemy) {
    enemy->Deactivate();
}

std::vector<Enemy*> Pooler::GetActiveEnemies() const {
    std::vector<Enemy*> activeEnemies;
    activeEnemies.reserve(enemies.size());

    for (Enemy* enemy : enemies) {
        if (enemy->IsActive()) {
            activeEnemies.push_back(enemy);
        }
    }

    return activeEnemies;
}

void Pooler::Clear() {
    for (Enemy* enemy : enemies) {
        enemy->Deactivate();
    }

    while (enemies.size() > minimumPoolSize) {
        delete enemies.back();
        enemies.pop_back();
    }
}

Enemy* Pooler::CreateEnemy() {
    return new Enemy();
}
