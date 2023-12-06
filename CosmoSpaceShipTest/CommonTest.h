#pragma once

#include <cstdint>

//! режим зполнения тестовых данных
// enum class FillTestDataMode
// {
//   //! Заполнения минимальным значениями
//   FillWithZeros = 0,
//   //! Заполнение максимальными значениями
//   FillInUnits = 1,
//   //! Заполняния случайными значениями от мнинмального до максимального
//   FillWithRandomVal = 2,
// };

//! Процедура для заполнения структуры значениями
// template<typename T>
// int
// ProcessingTest(T& actual, FillTestDataMode x_mode, T& expected)
// {
//   initStruct(actual, x_mode);

//   auto vecStruct = serialize(actual);
//   expected = deserialize<T>(vecStruct);
//   auto masByte = countingSize<T>();
//   return static_cast<int>(masByte[0]);
// }