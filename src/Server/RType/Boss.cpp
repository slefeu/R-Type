#include <ECS/Components/AnimationComponent.hpp>
#include <ECS/Components/BehaviorComponent.hpp>
#include <ECS/Components/DestroyableComponent.hpp>
#include <ECS/Components/DrawableComponent.hpp>
#include <ECS/Components/HitboxComponent.hpp>
#include <ECS/Components/InputComponent.hpp>
#include <ECS/Components/MouvementComponent.hpp>
#include <ECS/Components/StatComponent.hpp>
#include <ECS/Components/TimerComponent.hpp>
#include <ECS/Components/TransformComponent.hpp>
#include <Server/RType.hpp>
#include <Tools/Curve.hpp>
#include <cmath>
#include <vector>

int RType::createBoss(int x, int y) noexcept
{
    auto Boss = entityManager_.newEntity();
    nbEnemyAlive += 1;
    std::vector<std::vector<int>> bulletpos = {{-1, 0}, {0, -1}, {-1, 1}, {1, -1}, {1, 1}, {0, 1}, {1, 0}, {-1, -1}};

    Boss->addComponent(DrawableComponent(0, 0, 16, 16, 35));
    Boss->addComponent(TransformComponent(x, y));
    Boss->addComponent(DestroyableComponent());
    Boss->addComponent(StatComponent({30, 2, 6, 1}));
    Boss->addComponent(MouvementComponent(-1, 0, 0));
    Boss->addComponent(TimerComponent(0.8));

    auto hitbox = HitboxComponent(16, 16);

    hitbox.setOnCollision(
        std::function<void(Entity * entity, Entity * me)>{[](Entity* entity, Entity* me) { return; }});

    auto behavior = BehaviorComponent();

    behavior.setOnUpdate(std::function<void(Entity * entity)>{[this, y, bulletpos](Entity* entity) {
        auto  dest    = entity->getComponent<DestroyableComponent>();
        auto  stat    = entity->getComponent<StatComponent>();
        auto  trans   = entity->getComponent<TransformComponent>();
        auto& timer   = entity->getComponent<TimerComponent>()->getTimer();
        auto  scaling = entity->getComponent<DrawableComponent>();
        auto x = trans->getX();
        auto y = trans->getY();

        scaling->setScale(2, 2);
        centerx = 120 + radius * cos(m_angle);
        centery = 120 + radius * sin(m_angle);
        trans->setPos(centerx, centery);
        m_angle += 0.1;
        if (m_angle >= 2 * M_PI) { m_angle = 0; }

        if (timer.isOver()) {
            auto bDamage = stat->getStat(RTypeStats::Damage);
            auto bSpeed  = stat->getStat(RTypeStats::Speed);
            auto bSize   = stat->getStat(RTypeStats::Size);
            auto xs      = (trans->getX() + 16);
            auto ys      = (trans->getY() + 4);

            for (int i = 0; i != 8; i++) {
                createBossBullet(xs, ys, bDamage, bSpeed, bSize, rand() % 2 == 0, bulletpos[i][0], bulletpos[i][1]);
            }
        }
        if (stat->getStat(RTypeStats::Life) <= 0) {
            if (rand() % 1 == 0) { createPowerUp(trans->getX(), trans->getY(), rand() % 2); }
            nbEnemyAlive -= 1;
            playerLevel += 1;
            dest->destroy();
        }
        if (x <= -16) { trans->setX(255); }
    }});

    Boss->addComponent(hitbox);
    Boss->addComponent(behavior);

    return Boss->getId();
}

int RType::createBossBullet(int x, int y, float damage, float speed, float size, bool type, int xp, int yp) noexcept
{
    auto  bullet = entityManager_.newEntity();
    float life   = type ? 1.0 : 200.0;

    bullet->addComponent(TransformComponent(x - 8, y + 4));
    bullet->addComponent(MouvementComponent(-1, 0, speed));
    bullet->addComponent(DrawableComponent(0, 0, 8, 8, type ? 6 : 5, size, size));
    bullet->addComponent(AnimationComponent(16, 0.1));
    bullet->addComponent(DestroyableComponent());
    bullet->addComponent(StatComponent({life, damage}));

    auto hitbox = HitboxComponent(8, 8);
    hitbox.setOnCollision(std::function<void(Entity * entity, Entity * me)>{[](Entity* entity, Entity* me) {
        if (entity->hasComponents<DestroyableComponent, HitboxComponent, StatComponent, InputComponent>()) {
            auto stat   = entity->getComponent<StatComponent>();
            auto statMe = me->getComponent<StatComponent>();
            auto destMe = me->getComponent<DestroyableComponent>();

            auto life = stat->getStat(RTypeStats::Life);
            auto dmg  = statMe->getStat(RTypeStats::Damage);
            stat->setStat(RTypeStats::Life, life - dmg);
            destMe->destroy();
        }
    }});

    auto behavior = BehaviorComponent();
    behavior.setOnUpdate(std::function<void(Entity * entity)>{[xp, yp](Entity* entity) {
        auto trans = entity->getComponent<TransformComponent>();
        auto dest  = entity->getComponent<DestroyableComponent>();
        auto stat  = entity->getComponent<StatComponent>();
        auto move  = entity->getComponent<MouvementComponent>();

        move->setDir(xp, yp);
        if (trans->getX() <= 0 || stat->getStat(RTypeStats::Life) <= 0) { dest->destroy(); }
    }});

    bullet->addComponent(hitbox);
    bullet->addComponent(behavior);

    return bullet->getId();
}
