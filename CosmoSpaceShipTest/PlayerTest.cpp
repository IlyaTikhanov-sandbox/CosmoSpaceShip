// #include "../src/Engine/Player.h"
// #include "../src/Engine/Common.h"
// #include "CommonTest.h"
// #include "gtest/gtest.h"

// // Фикстуры ClassPlayer
// class PlayerTest : public ::testing::TestWithParam<int>
// {
// public:
//   // Экземпляр класса player
//   Player player{ "User", 0, COMMON::EPlayerType::HUMAN };

//   // Ожидаемое значение имени
//   std::string expected_name = "Sanya";
//   // Пустая строка
//   std::string empty_name = "";
//   // Ожидаемыйы максимально возможный счёт 64х64 - 2 = 4094
//   const uint16_t expected_max_score = 4094;
//   const uint16_t expected_min_score = 0;
//   const uint16_t expected_begin_score = 16;
//   const int16_t negative_score = -2047;
//   const uint16_t pos_exceed_score = 4095;
//   const int16_t neg_exceed_score = -4095;
// };

// // Подаваемые положительные значения
// INSTANTIATE_TEST_SUITE_P(Positive,
//                          PlayerTest,
//                          ::testing::Values(COMMON::EPlayerType::HUMAN,
//                                            COMMON::EPlayerType::COMPUTER));

// TEST_P(PlayerTest, CheckingPositiveValSetEssence)
// {
//   // Получение возможнных параметров
//   auto xmode = static_cast<COMMON::EPlayerType>(GetParam());
//   // Проверка возвращаемой сущности
//   ASSERT_TRUE(player.SetEssence(xmode));
//   // Сравниваем установленный параметр с возвращаемым значением
//   ASSERT_EQ(xmode, player.GetEssence());
// }

// TEST_F(PlayerTest, CheckingNegativeVal)
// {
//   // Ожидаемое возвращаемое значение false при неверных параметрах
//   ASSERT_FALSE(player.SetEssence(static_cast<COMMON::EPlayerType>(3)));
//   // Ожидаемое возвращаемое значение false при пустой строке
//   ASSERT_FALSE(player.SetName(empty_name));
//   // Установим начальный счёт больше максимально возможного
//   ASSERT_FALSE(player.SetBeginScore(pos_exceed_score));
//   // Добавим к счёту по умолчанию значение которое приведёт к результату < 0
//   ASSERT_FALSE(player.AddPoints(neg_exceed_score));
// }

// TEST_F(PlayerTest, CheckingInputData)
// {
//   // Устанавливаем имя
//   player.SetName(expected_name);
//   // Проверяем возращаемое значение
//   ASSERT_EQ(expected_name, player.GetName());

//   // Проверим начальное значение счётчика
//   ASSERT_EQ(expected_min_score, player.GetScore());

//   // Добавляемый к счёту максимальное значение / 2
//   ASSERT_TRUE(player.AddPoints(expected_max_score / 2));
//   // Сравниваем записанное значение с возвращаемым результатом
//   ASSERT_EQ((expected_max_score / 2), player.GetScore());
//   // Добавляемый к счёту максимальное значение / 2
//   ASSERT_TRUE(player.AddPoints(expected_max_score / 2));
//   // На выходе ожидаем максимальное значение
//   ASSERT_EQ(expected_max_score, player.GetScore());
//   // Добавляемый к счёту отрицательное значение == максимальное значение / 2
//   ASSERT_TRUE(player.AddPoints(negative_score));
//   // На выходе ожидаем максимальное значение
//   ASSERT_EQ((expected_max_score / 2), player.GetScore());

//   // Устанавливаем начальный счёт
//   ASSERT_TRUE(player.SetBeginScore(expected_begin_score));
//   // Сравниваем записанное значение со счётом
//   ASSERT_EQ(expected_begin_score, player.GetScore());
// }
