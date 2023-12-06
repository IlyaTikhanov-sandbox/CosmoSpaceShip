// #include "../src/Engine/MultiplayerMode.h"
// #include "../src/Engine/Common.h"
// #include "../src/Engine/Field.h"
// #include "../src/Engine/IGame.h"
// #include "../src/Engine/Player.h"
// #include "CommonTest.h"
// #include "gtest/gtest.h"

// // Фикстуры SingleMode
// class MultiplayerModeTest : public ::testing::TestWithParam<int>
// {
// public:
//   // Создаём одиночную игру
//   MultiplayerMode multiplayer_mode;
//   // Экземпляр класса Field
//   Field field;
//   // Экземпляры игроков
//   Player player_1{ "Alex", 0, COMMON::EPlayerType::HUMAN };
//   Player player_2{ "Dima", 0, COMMON::EPlayerType::HUMAN };
//   Player player_3{ "Lexa", 0, COMMON::EPlayerType::HUMAN };

//   // Список игроков
//   std::vector<Player> ListPlayer;
//   std::vector<Player> ListGetPlayer;
//   // Направления движения
//   std::vector<COMMON::EDirect> direct;
// };

// TEST_F(MultiplayerModeTest, CheckingPositiveCreateNewGame)
// {
//   // Передадим в список игроков
//   ListPlayer.push_back(player_1);
//   ListPlayer.push_back(player_2);
//   ListPlayer.push_back(player_3);
//   // Передаём интерфейсу список;
//   multiplayer_mode.SetPlayerList(ListPlayer);
//   // Получим этот список и проверим что список игроков == 3
//   ASSERT_EQ(3, multiplayer_mode.GetPlayerList().size());
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим поле
//   multiplayer_mode.SetField(field);
//   // Попробуем создать новую игру
//   ASSERT_TRUE(multiplayer_mode.CreateNewGame());
// }

// TEST_F(MultiplayerModeTest, CheckingNegativePlayersCreateNewGame)
// {
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим поле
//   multiplayer_mode.SetField(field);
//   // Попробуем создать новую игру без игроков
//   ASSERT_FALSE(multiplayer_mode.CreateNewGame());
// }

// TEST_F(MultiplayerModeTest, CheckingNegativeFieldCreateNewGame)
// {
//   // Передадим поле
//   multiplayer_mode.SetField(field);
//   // Попробуем создать новую игру без поля и игроков
//   ASSERT_FALSE(multiplayer_mode.CreateNewGame());
// }

// TEST_F(MultiplayerModeTest, CheckingPositiveDoMove)
// {
//   // Передадим в список игроков
//   ListPlayer.push_back(player_1);
//   ListPlayer.push_back(player_2);
//   ListPlayer.push_back(player_3);
//   // Передаём интерфейсу список;
//   multiplayer_mode.SetPlayerList(ListPlayer);
//   // Получим этот список и проверим что список игроков == 3
//   ASSERT_EQ(3, multiplayer_mode.GetPlayerList().size());
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим поле
//   multiplayer_mode.SetField(field);
//   // Попробуем создать новую игру
//   ASSERT_TRUE(multiplayer_mode.CreateNewGame());
//   // Проверим что ходит сейчас первый игрок
//   ASSERT_EQ(0, multiplayer_mode.GetActivePlayerId());
//   // Проверим счёт игрока
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(0, ListGetPlayer[0].GetScore());
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
//   ASSERT_TRUE(multiplayer_mode.DoMove(1, 3, direct));
//   // Проверим счёт игрока 0
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(1, ListGetPlayer[0].GetScore());

//   // Попробуем ещё раз сделать ход направо, что невозможно
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • o o X • • •
//   //  4 • • • • • • •
//   //  5     • • •
//   //  6     • • •
//   ASSERT_FALSE(multiplayer_mode.DoMove(3, 3, direct));

//   // Проверим, что ход перешёл следующему игроку
//   ASSERT_EQ(1, multiplayer_mode.GetActivePlayerId());
//   // Проверим счёт игрока
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(0, ListGetPlayer[1].GetScore());
//   //  Выберем фишку x = 4, y = 3
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • o o • X • •
//   //  4 • • • • • • •
//   //  5     • • •
//   //  6     • • •
//   // Зададим направление налево
//   direct = { COMMON::EDirect::LEFT };
//   // Попробуем сделать ход
//   ASSERT_TRUE(multiplayer_mode.DoMove(4, 3, direct));
//   // Проверим счёт игрока 1
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(1, ListGetPlayer[1].GetScore());

//   // Сделаем какой нибудь невозможный ход, например попробуем походить из
//   // координаты x = 6  y = 6
//   ASSERT_FALSE(multiplayer_mode.DoMove(6, 6, direct));

//   // Проверим, что ход перешёл следующему игроку
//   ASSERT_EQ(2, multiplayer_mode.GetActivePlayerId());
//   // Проверим счёт игрока
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(0, ListGetPlayer[2].GetScore());
//   //  Выберем фишку x = 4, y = 5
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • o • o o • •
//   //  4 • • • • • • •
//   //  5     • • X
//   //  6     • • •
//   // Зададим направление вверх
//   direct = { COMMON::EDirect::UP };
//   // Попробуем сделать ход
//   ASSERT_TRUE(multiplayer_mode.DoMove(4, 5, direct));
//   // Проверим счёт игрока 2
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(1, ListGetPlayer[2].GetScore());

//   // Проверим, что ход перешёл первому игроку от последнего
//   ASSERT_EQ(0, multiplayer_mode.GetActivePlayerId());
//   // Сразу проверим возможность мультихода
//   //  Выберем фишку x = 5, y = 3
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • o • o • X •
//   //  4 • • • • o • •
//   //  5     • • o
//   //  6     • • •
//   // Сначала попробуем сделать сразу три хода влева, чтобы проверить что
//   // это невозможно
//   direct = { COMMON::EDirect::LEFT,
//              COMMON::EDirect::LEFT,
//              COMMON::EDirect::LEFT };

//   // Попробуем сделать неправильный ход
//   ASSERT_FALSE(multiplayer_mode.DoMove(5, 3, direct));
//   // Проверим что счёт игрока при этом не изменился
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(1, ListGetPlayer[0].GetScore());

//   // Поменяем направление на два раза влево
//   direct = { COMMON::EDirect::LEFT, COMMON::EDirect::LEFT };
//   // Попробуем сделать неправильный ход
//   ASSERT_TRUE(multiplayer_mode.DoMove(5, 3, direct));
//   // Счёт игрока 0 должен быть 1 + 2 = 3
//   ListGetPlayer = multiplayer_mode.GetPlayerList();
//   ASSERT_EQ(3, ListGetPlayer[0].GetScore());
// }