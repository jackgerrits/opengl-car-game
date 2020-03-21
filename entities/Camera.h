#pragma once

#include "../constants.h"

#include "../InputState.h"
#include "Player.h"

#include <glm/glm.hpp>

class Camera {
  protected:
    glm::vec3 m_position{0.f};
    glm::vec3 m_focal_point{0.f};
    glm::mat4 m_view_matrix{1.f};

  public:
    Camera() = default;

    [[nodiscard]] const glm::mat4& getViewMtx() const;
    [[nodiscard]] const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3& position);
    [[nodiscard]] glm::mat4 getInverted(float pivotPoint);

    virtual void update(InputState& input) = 0;

    void look(const glm::vec3& at);
    void look(const glm::vec3& from, const glm::vec3& at);
};

class PlayerCamera : public Camera {
  private:
    Player* m_player;
    float m_distance;
    float m_pitch;
    float m_angle_around;

    static constexpr float RESET_SPEED = constants::PI / 2.f;

  public:
    PlayerCamera(Player* player);
    void update(InputState& input) override;
};
