// #include "../src/Engine/Field.h"
// #include "../src/Engine/Common.h"
// #include "CommonTest.h"
// #include "gtest/gtest.h"

// // Фикстуры ClassField
// class FieldTest : public ::testing::TestWithParam<int>
// {
// public:
//   // Экземпляр класса Field
//   Field field;
//   // Ожидаемое поле
//   std::vector<COMMON::ECell> field_expected;
//   // Размер для EN и EU полей 7x7
//   const int Size = 49;
// };

// // Подаваемые значения для выбора режима
// INSTANTIATE_TEST_SUITE_P(ModeL,
//                          FieldTest,
//                          ::testing::Values(COMMON::ETypeField::ENGLISH,
//                                            COMMON::ETypeField::EUROPEAN));

// TEST_P(FieldTest, CheckingPositiveTheCreationOfFields)
// {
//   // Получение возможнных параметров
//   auto xmode = static_cast<COMMON::ETypeField>(GetParam());
//   // Создание поля с для английской версии
//   ASSERT_TRUE(field.Create(xmode));
//   // Получить это поле
//   field_expected = field.GetField();
//   // Проверяем соответствие размеров
//   ASSERT_EQ(field_expected.size(), Size);
// }

// TEST_F(FieldTest, CheckingNegativeTheCreationOfFields)
// {
//   // Создание поля с неверным параметром
//   ASSERT_FALSE(field.Create(static_cast<COMMON::ETypeField>(3)));
// }

// TEST_F(FieldTest, CheckingGetSize)
// {
//   ASSERT_TRUE(field.Create(COMMON::ETypeField::ENGLISH));
//   // По дефолту параметры w = 7 и h = 7
//   ASSERT_EQ(7, field.GetWidth());
//   ASSERT_EQ(7, field.GetHeight());
// }