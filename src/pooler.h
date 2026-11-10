//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#ifndef POOLER_H
#define POOLER_H

#include <memory>
#include <list>
#include <vector>

#include "enemy.h"

class Pooler {
   public:
    Pooler(size_t minimumPoolSize);
    ~Pooler();

    size_t PoolSize();

    Enemy* SpawnEnemy(const Vector2D& spawnPosition);
    void DespawnEnemy(Enemy* enemy);
    std::vector<Enemy*> GetActiveEnemies() const;
    void Clear();

   private:
    std::list<Enemy*> enemies;
    size_t minimumPoolSize;

    Enemy* CreateEnemy();
};

#endif  // POOLER_H