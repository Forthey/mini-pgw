# PGW сервер

---

Сервер позволяет
- Обрабатывать UDP-запросы.
- Управлять сессиями абонентов по IMSI.
- Вести CDR-журнал.
- Предоставлять HTTP API.
- Поддерживать чёрный список IMSI.

---

## Сборка

---

Debug
```shell
cmake -S . -B build && cmake --build build --config Debug   
```

Release
```shell
cmake -S . -B build && cmake --build build --config Release   
```

После выполнения команды исполняемый файл будет находиться в директории bin 

Использование:

```shell
./server <путь к .json конфигу>
```
