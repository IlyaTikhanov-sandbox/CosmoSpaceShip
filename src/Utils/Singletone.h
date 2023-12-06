/**
 * @file Singletone.h
 * @brief Шаблонный класс реализующий паттерн Одиночка
 */
#pragma once

/**
 * @brief Шаблонный класс реализующий паттерн Одиночка
 *
 * @tparam T Тип экземпляра одиночки
 */
template<typename T>
class Singleton
{
public:
  /**
   * @brief Предоставляет ссылку на экземпляр указанного класса
   *
   * @return Ссылка на экземпляр указанного класса
   */
  static T& GetInstance()
  {
    static T instance;
    return instance;
  }

protected:
  /**
   * @brief Конструктор
   */
  Singleton() = default;
  /**
   * @brief Деструктор
   */
  ~Singleton() = default;

public:
  /**
   * @brief Запрет конструктора копирования
   */
  Singleton(Singleton const&) = delete;
  /**
   * @brief Запрет конструктора перемещения
   */
  Singleton(Singleton&&) = delete;
  /**
   * @brief Запрет копирования присваиванием
   */
  Singleton& operator=(Singleton const&) = delete;
  /**
   * @brief Запрет перемещения присваиванием
   */
  Singleton& operator=(Singleton&&) = delete;
};