//Copyright 2K Games, Engineering Graduate Program. All rights reserved. DO NOT redistribute.

#include "tower.h"
#include "logger.h"

#include <algorithm>
#include <cmath>
#include <limits>

Tower::Tower(int x, int y, int rateOfFire)
    : position({x, y}),
      lastFireTime(std::chrono::steady_clock::now()),
      rateOfFire(rateOfFire),
      bAutoFire(true)
    {}

void Tower::Update(std::vector<Enemy*> enemies,
                   std::vector<Bullet>& bullets) {
    if (bAutoFire) {
        FireAtEnemy(enemies, bullets);
    }
}

void Tower::FireAtEnemy(const std::vector<Enemy*>& enemies,
                        std::vector<Bullet>& bullets) {
    auto currentTime = std::chrono::steady_clock::now();
    auto timeElapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastFireTime);

    if (timeElapsed < std::chrono::milliseconds(rateOfFire)) {
        return;
    }

    Enemy* closestEnemy = FindClosestEnemy(enemies);
    if (closestEnemy == nullptr) {
        return;
    }

    // Auto-fire respects ENEMYRANGE; manual fire always targets the closest
    // enemy regardless of distance, matching the original control scheme.
    if (bAutoFire && CalculateDistance(closestEnemy->GetPosition(), position) >
                         static_cast<int>(ENEMYRANGE)) {
        return;
    }

    FireBulletAtEnemy(*closestEnemy, bullets);
    lastFireTime = currentTime;
}

int Tower::CalculateDistance(const Vector2D& from, const Vector2D& to) const {
    int deltaX = std::abs(from.x - to.x);
    int deltaY = std::abs(from.y - to.y);

    return std::max(deltaX, deltaY);
}

Enemy* Tower::FindClosestEnemy(const std::vector<Enemy*>& enemies) const {
    Enemy* closestEnemy = nullptr;
    int closestDistance = std::numeric_limits<int>::max();

    for (Enemy* enemy : enemies) {
        int distance = CalculateDistance(enemy->GetPosition(), position);

        if (distance < closestDistance) {
            closestDistance = distance;
            closestEnemy = enemy;
        }
    }

    return closestEnemy;
}

bool Tower::GetAutoFire() const { return bAutoFire; }

void Tower::SetAutoFire(bool autoFire) { bAutoFire = autoFire; }

void Tower::ManualFire(std::vector<Enemy*> enemies,
                       std::vector<Bullet>& bullets) {
    FireAtEnemy(enemies, bullets);
}

void Tower::FireBulletAtEnemy(Enemy& enemy, std::vector<Bullet>& bullets) {
    Vector2D direction = {0, 0};
    if (enemy.GetPosition().x > position.x) direction.x = 1;
    if (enemy.GetPosition().x < position.x) direction.x = -1;
    if (enemy.GetPosition().y > position.y) direction.y = 1;
    if (enemy.GetPosition().y < position.y) direction.y = -1;

    bullets.emplace_back(position, direction);
}