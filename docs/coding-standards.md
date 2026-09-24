# Стандарты кодирования проекта MOGGулятор3000 (C++17)

## 1. Именование элементов кода

- **Переменные**: `camelCase` (например, `rawExpression`, `calculatedResult`).
- **Поля классов**: Префикс `m_` + `camelCase` (например, `m_expression`, `m_steps`).
- **Константы**: `UPPER_SNAKE_CASE` (например, `MAX_PRECISION = 6`, `DEFAULT_LANGUAGE = "rus+eng"`).
- **Функции и методы**: `camelCase` (например, `normalize()`, `buildSteps()`, `save()`).
- **Классы и структуры**: `PascalCase` (например, `MathExpression`, `HistoryEntry`, `StepGenerator`).
- **Пространство имён**: `namespace mogg`.
- **Файлы исходного кода**: `snake_case.h` и `snake_case.cpp` (например, `math_expression.h`, `step_generator.cpp`).

## 2. Правила оформления

- В заголовочных файлах обязательно использовать директиву `#pragma once`.
- Отступы — 4 пробела (символы табуляции запрещены).
- Кодировка всех файлов — UTF-8.
- Переводы строк — LF.
