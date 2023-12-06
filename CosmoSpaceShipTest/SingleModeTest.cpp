// #include "../src/Engine/SingleMode.h"
// #include "../src/Engine/Common.h"
// #include "../src/Engine/Field.h"
// #include "../src/Engine/IGame.h"
// #include "../src/Engine/Player.h"
// #include "CommonTest.h"
// #include "gtest/gtest.h"

// // Фикстуры SingleMode
// class SingleModeTest : public ::testing::TestWithParam<int>
// {
// public:
//   // Создаём одиночную игру
//   SingleMode singlemode;
//   // Экземпляр класса Field
//   Field field;
//   // Экземпляры игроков
//   Player player_1{ "Alex", 0, COMMON::EPlayerType::HUMAN };

//   // Список игроков
//   std::vector<Player> ListPlayer;
//   std::vector<Player> ListGetPlayer;
//   // Направления движения
//   std::vector<COMMON::EDirect> direct;
// };

// TEST_F(SingleModeTest, CheckingPositiveCreateNewGame)
// {
//   // Передадим в список одного игрока
//   ListPlayer.push_back(player_1);
//   // Передаём интерфейсу список;
//   singlemode.SetPlayerList(ListPlayer);
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим поле
//   singlemode.SetField(field);
//   // Попробуем создать новую игру
//   ASSERT_TRUE(singlemode.CreateNewGame());
// }

// TEST_F(SingleModeTest, CheckingNegativePlayerCreateNewGame)
// {
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим поле
//   singlemode.SetField(field);
//   // Попробуем создать новую игру без игрока
//   ASSERT_FALSE(singlemode.CreateNewGame());
// }

// TEST_F(SingleModeTest, CheckingNegativeFieldCreateNewGame)
// {
//   // Передадим поле
//   singlemode.SetField(field);
//   // Попробуем создать новую игру без поля и игрока
//   ASSERT_FALSE(singlemode.CreateNewGame());
// }

// TEST_F(SingleModeTest, CheckingPositiveDoMove)
// {
//   // Передадим в список одного игрока
//   ListPlayer.push_back(player_1);
//   // Передаём интерфейсу список;
//   singlemode.SetPlayerList(ListPlayer);
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим поле
//   singlemode.SetField(field);
//   // Попробуем создать новую игру
//   ASSERT_TRUE(singlemode.CreateNewGame());
//   // Проверим счёт игрока
//   ListGetPlayer = singlemode.GetPlayerList();
//   ASSERT_EQ(32, ListGetPlayer[0].GetScore());
//   //  Выберем фишку x = 1, y = 3
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • X • o • • •
//   //  4 • • • • • • •
//   //  5     • • •
//   //  6     • • •
//   //  Зададим одно направление направо
//   direct = { COMMON::EDirect::RIGHT };
//   // Попробуем сделать ход
//   ASSERT_TRUE(singlemode.DoMove(1, 3, direct));
//   // Проверим счёт игрока
//   ListGetPlayer = singlemode.GetPlayerList();
//   ASSERT_EQ(31, ListGetPlayer[0].GetScore());

//   // Попробуем ещё раз сделать ход направо, что невозможно
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • o o X • • •
//   //  4 • • • • • • •
//   //  5     • • •
//   //  6     • • •
//   ASSERT_FALSE(singlemode.DoMove(3, 3, direct));
// }

// TEST_F(SingleModeTest, CheckingPositiveMultiDoMove)
// {
//   // Передадим в список одного игрока
//   ListPlayer.push_back(player_1);
//   // Передаём интерфейсу список;
//   singlemode.SetPlayerList(ListPlayer);
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   //  Уберём фишки из координат помеченных Y для проверки мультихода
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • X • Y • •
//   //  3 • • • 0 • • •
//   //  4 • • Y • Y • •
//   //  5     • • •
//   //  6     • • •
//   field.SetCell(4, 2, COMMON::ECell::FREE);
//   field.SetCell(4, 4, COMMON::ECell::FREE);
//   field.SetCell(2, 4, COMMON::ECell::FREE);
//   // Передадим поле
//   singlemode.SetField(field);
//   // Попробуем создать новую игру
//   ASSERT_TRUE(singlemode.CreateNewGame());
//   // Установим направление для мультихода
//   direct = {
//     COMMON::EDirect::RIGHT, COMMON::EDirect::DOWN, COMMON::EDirect::LEFT, COMMON::EDirect::UP
//   };
//   // Попробуем сделать ход
//   ASSERT_TRUE(singlemode.DoMove(2, 2, direct));
//   // Проверим счёт игрока
//   ListGetPlayer = singlemode.GetPlayerList();
//   ASSERT_EQ(25, ListGetPlayer[0].GetScore());
//   // Попробуем ещё раз сделать ход направо, что невозможно
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • X o o • •
//   //  3 • • o o o • •
//   //  4 • • o o o • •
//   //  5     • • •
//   //  6     • • •
//   ASSERT_FALSE(singlemode.DoMove(3, 3, direct));
// }