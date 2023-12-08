#include <SFML/Graphics.hpp>
#include <glog/logging.h>
#include <iostream>

bool
checkCollision(const sf::CircleShape& circle1, const sf::CircleShape& circle2)
{
  // Получаем позиции центров окружностей
  sf::Vector2f circle1Pos = circle1.getPosition();
  sf::Vector2f circle2Pos = circle2.getPosition();

  // Получаем радиусы окружностей
  float radius1 = circle1.getRadius();
  float radius2 = circle2.getRadius();

  // Рассчитываем вектор между центрами окружностей
  sf::Vector2f distanceVec = circle2Pos - circle1Pos;

  // Вычисляем расстояние между центрами окружностей
  float distance = std::sqrt(distanceVec.x * distanceVec.x + distanceVec.y * distanceVec.y);

  // Проверяем, пересекаются ли окружности
  if (distance <= radius1 + radius2) {
    return true; // Коллизия
  } else {
    return false; // Нет коллизии
  }
}

int
main(int argc, char* argv[])
{
#pragma region "Init"
  google::InitGoogleLogging(argv[0]);
  FLAGS_alsologtostderr = true;
  FLAGS_log_dir = "..\\..\\..\\log";
  LOG(INFO) << "ZARA SEE YOU!";
#pragma endregion

  sf::RenderWindow window(sf::VideoMode(800, 600), "Circle Collider SFML");

  sf::CircleShape circle1(50.f);
  circle1.setFillColor(sf::Color::Blue);
  circle1.setPosition(100.f, 100.f);
  sf::Vector2f circle1Velocity(100.f, 80.f); // Пикселей в секунду

  sf::CircleShape circle2(70.f);
  circle2.setFillColor(sf::Color::Red);
  circle2.setPosition(300.f, 300.f);
  sf::Vector2f circle2Velocity(-120.f, -90.f); // Пикселей в секунду

  sf::Clock clock;

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    float deltaTime = clock.restart().asSeconds();

    // Обновляем позиции кругов на основе скорости и времени
    circle1.move(circle1Velocity * deltaTime);
    circle2.move(circle2Velocity * deltaTime);

    // Ограничиваем движение кругов в пределах окна и меняем направление при столкновении с
    // границами
    sf::Vector2f circle1Pos = circle1.getPosition();
    sf::Vector2f circle2Pos = circle2.getPosition();

    if (circle1Pos.x <= 0 || circle1Pos.x >= window.getSize().x - 2 * circle1.getRadius())
      circle1Velocity.x = -circle1Velocity.x;
    if (circle1Pos.y <= 0 || circle1Pos.y >= window.getSize().y - 2 * circle1.getRadius())
      circle1Velocity.y = -circle1Velocity.y;

    if (circle2Pos.x <= 0 || circle2Pos.x >= window.getSize().x - 2 * circle2.getRadius())
      circle2Velocity.x = -circle2Velocity.x;
    if (circle2Pos.y <= 0 || circle2Pos.y >= window.getSize().y - 2 * circle2.getRadius())
      circle2Velocity.y = -circle2Velocity.y;

    // Проверяем коллизию
    if (checkCollision(circle1, circle2)) {
      LOG(WARNING) << "Collision detected!";
      circle1.setFillColor(sf::Color::Green);
      circle2.setFillColor(sf::Color::Yellow);

      // Меняем направление движения при коллизии (можно также реализовать более сложную логику)
      sf::Vector2f temp = circle1Velocity;
      circle1Velocity = circle2Velocity;
      circle2Velocity = temp;
    } else {
      circle1.setFillColor(sf::Color::Blue);
      circle2.setFillColor(sf::Color::Red);
    }

    window.clear();
    window.draw(circle1);
    window.draw(circle2);
    window.display();
  }

  return 0;
}