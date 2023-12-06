// #include "../src/Engine/IGame.h"
// #include "../src/Engine/Common.h"
// #include "../src/Engine/Field.h"
// #include "../src/Engine/Player.h"
// #include "CommonTest.h"
// #include "gtest/gtest.h"

// // Фикстуры ClassIGame
// class IGameTest : public ::testing::TestWithParam<int>
// {
// public:
//   // Экземпляр класса IGame
//   IGame igame;
//   // Экземпляр класса Field
//   Field field;
//   Field field_copy;
//   // Вектор поля
//   std::vector<COMMON::ECell> v_original;
//   std::vector<COMMON::ECell> v_expected;
//   // Направления движения
//   std::vector<COMMON::EDirect> direct;

//   // Экземпляры игроков
//   Player player_1{ "Alex", 0, COMMON::EPlayerType::HUMAN };
//   Player player_2{ "Dima", 0, COMMON::EPlayerType::HUMAN };
//   Player player_3{ "Lexa", 0, COMMON::EPlayerType::HUMAN };

//   // Список игроков
//   std::vector<Player> ListPlayerActual;
//   std::vector<Player> ListPlayerExpected;
// };

// TEST_F(IGameTest, CheckingSetPlayerList)
// {
//   // Загружаем список игроков
//   ListPlayerActual.push_back(player_1);
//   ListPlayerActual.push_back(player_2);
//   ListPlayerActual.push_back(player_3);
//   // Передаём интерфейсу список
//   igame.SetPlayerList(ListPlayerActual);
//   // Получаем его из класса
//   ListPlayerExpected = igame.GetPlayerList();
//   // Проверяем соответствие векторов
//   ASSERT_TRUE(ListPlayerActual == ListPlayerExpected);
// }

// TEST_F(IGameTest, CheckingGetField)
// {
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим класс IGame поле
//   igame.SetField(field);
//   // скопируем поле в вектор из оригинала
//   v_original = field.GetField();
//   // вернём объект field из game
//   field_copy = igame.GetField();
//   // скопируем поле в вектор из копии
//   v_expected = field_copy.GetField();
//   // сравним вектора
//   ASSERT_EQ(v_original, v_expected);
// }

// TEST_F(IGameTest, CheckingFieldMove)
// {
//   // Создадим поле для английской версии
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // Передадим класс IGame поле
//   igame.SetField(field);

//   // Проверим что не конец игры
//   ASSERT_TRUE(!igame.IsGameOver());

//   //  Выберем фишку x = 5, y = 3
//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • • • o • X •
//   //  4 • • • • • • •
//   //  5     • • •
//   //  6     • • •
//   //  Зададим одно направление влево

//   // Установим направление влево
//   direct = { COMMON::EDirect::LEFT };
//   // Проверим что туда можно походить
//   ASSERT_TRUE(igame.CheckMove(5, 3, direct));
//   // Установим значение направо
//   direct = { COMMON::EDirect::RIGHT };
//   // Проверим что туда нельзя походить
//   ASSERT_FALSE(igame.CheckMove(5, 3, direct));

//   // Проверим явно недопустимые варианты
//   ASSERT_FALSE(igame.CheckMove(3, 3, direct));
//   ASSERT_FALSE(igame.CheckMove(6, 6, direct));

//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • • • • • • •
//   //  3 • • • Х o o •
//   //  4 • • • • • • •
//   //  5     • • •
//   //  6     • • •

//   // Установим направление влево
//   direct = { COMMON::EDirect::LEFT };
//   // Сделаем ход
//   ASSERT_TRUE(igame.DoMove(5, 3, direct));

//   // Проверим что влево нельзя походить из координаты 3,3
//   ASSERT_FALSE(igame.DoMove(3, 3, direct));

//   // Проверим что влево нельзя походить из координаты где нет фишки
//   ASSERT_FALSE(igame.DoMove(5, 3, direct));
// }

// TEST_F(IGameTest, CheckingFieldMultiMove)
// {
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

//   // Передадим класс IGame поле
//   igame.SetField(field);
//   // Проверим что не конец игры
//   ASSERT_TRUE(!igame.IsGameOver());

//   // Походим из X в Y(4,2) -> Y(4.4)->Y(2,4) -> 2,2

//   // Установим направление для мультихода
//   direct = { COMMON::EDirect::RIGHT,
//              COMMON::EDirect::DOWN,
//              COMMON::EDirect::LEFT,
//              COMMON::EDirect::UP };

//   // Проверим что туда можно походить
//   ASSERT_TRUE(igame.CheckMove(2, 2, direct));

//   // Делаем мультиход
//   ASSERT_TRUE(igame.DoMove(2, 2, direct));

//   // Проверяем, что направо ещё раз походить нельзя
//   direct = { COMMON::EDirect::RIGHT };

//   // Проверим что нельзя сделать ход
//   ASSERT_FALSE(igame.CheckMove(2, 2, direct));
//   // Попробуем сделать ход куда нельзя
//   ASSERT_FALSE(igame.DoMove(2, 2, direct));

//   //    0 1 2 3 4 5 6
//   //  0     • • •
//   //  1     • • •
//   //  2 • X Y 0 0 • •
//   //  3 • • 0 0 0 • •
//   //  4 • • 0 0 0 • •
//   //  5     • • •
//   //  6     • • •

//   // Попробуем походить вправо из координаты x = 1 y = 2
//   ASSERT_TRUE(igame.CheckMove(1, 2, direct));
//   // Попробуем туда сделать ход
//   ASSERT_TRUE(igame.DoMove(1, 2, direct));

//   // Попробуем сделать неправильный мультиход из х = 0 у = 3 вправо и ещё раз
//   // вправо
//   direct = { COMMON::EDirect::RIGHT, COMMON::EDirect::RIGHT };
//   // Проверим что это не возможно
//   ASSERT_FALSE(igame.CheckMove(0, 3, direct));
//   // Попробуем туда сделать ход
//   ASSERT_FALSE(igame.DoMove(0, 3, direct));
// }