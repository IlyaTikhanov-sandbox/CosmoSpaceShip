#include "Utils/AssetManager.h"
#include <SFML/Audio.hpp>
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
  FLAGS_log_dir = "..\\..\\log";
  LOG(INFO) << "ZARA SEE YOU!";
#pragma endregion

  std::string dbPath = "assets.db";
  // Создаем объект AssetManager, передавая путь к базе данных
  AssetManager assetManager(dbPath);

  // Получаем данные ассета по его имени
  std::string assetName = "Background Music";
  std::string assetNameImage = "Main Character";
  Asset asset = assetManager.GetAssetByName(assetName);
  Asset assetImg = assetManager.GetAssetByName(assetNameImage);

  // Выводим информацию о пути ассета, если он найден
  if (!asset.name.empty()) {
    LOG(INFO) << "Name asset: " << asset.name << std::endl;
    LOG(INFO) << "Path asset: " << asset.path << std::endl;
  } else {
    LOG(INFO) << "Asset with name: " << assetName << " not found!" << std::endl;
  }

  if (!assetImg.name.empty()) {
    LOG(INFO) << "Name asset: " << assetImg.name << std::endl;
    LOG(INFO) << "Path asset: " << assetImg.path << std::endl;
  } else {
    LOG(INFO) << "Asset with name: " << assetNameImage << " not found!" << std::endl;
  }

  sf::Music music;
  if (!music.openFromFile(asset.path)) {
    LOG(INFO) << "Error with audio!" << std::endl;
    return EXIT_FAILURE;
  }

  music.play();

  sf::Texture texture;
  if (!texture.loadFromFile(assetImg.path)) {
    LOG(INFO) << "Error with image!" << std::endl;
    return EXIT_FAILURE;
  }

  // Параметр скорости перемещения кругов (можете настроить его по своему усмотрению)
  const float circleVelocity = 90.f;

  sf::Vector2u videoMode = { 800, 600 };

  sf::RenderWindow window(sf::VideoMode(videoMode), "Circle Collider SFML");
  sf::RenderWindow window2(sf::VideoMode(videoMode), "SFML Image");

  window.setPosition(sf::Vector2i(000, 000));
  window2.setPosition(sf::Vector2i(900, 000));

  sf::Sprite sprite(texture);

  sf::Vector2u windowSize = window2.getSize();
  sf::Vector2u textureSize = texture.getSize();

  // Устанавливаем масштаб спрайта по осям X и Y
  sprite.setTextureRect(
    sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(window2.getSize().x, window2.getSize().y)));

  window.setFramerateLimit(60);

  sf::CircleShape circle1(50.f);
  circle1.setFillColor(sf::Color::Blue);
  sf::Vector2f pos_1 = { 100.f, 100.f };
  circle1.setPosition(pos_1);
  sf::Vector2f circle1Velocity(circleVelocity, circleVelocity); // Используем параметр скорости

  sf::CircleShape circle2(70.f);
  circle2.setFillColor(sf::Color::Red);
  sf::Vector2f pos_2 = { 300.f, 300.f };
  circle2.setPosition(pos_2);
  sf::Vector2f circle2Velocity(-circleVelocity, -circleVelocity); // Используем параметр скорости

  sf::Clock clock;
  sf::Time timePerFrame = sf::seconds(1.f / 60.f);

  sf::Time timeSinceLastUpdate = sf::Time::Zero;
  while (window.isOpen() || window2.isOpen()) {
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
      std::cout << "Collision detected!" << std::endl;
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
    window2.clear();
    window.draw(circle1);
    window.draw(circle2);
    window2.draw(sprite);
    window.display();
    window2.display();
  }

  return 0;
}