# PGW клиент

---

Клиент позволяет
- Отправлять UDP-запрос с IMSI.
- Получать и логгировать ответ о результате выполнения запроса

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
./client <путь к .json конфигу> <IMSI>
```

Или можно запустить сразу несколько клиентов со случайным IMEI:

```shell
./run_clients.bash <путь к .json конфигу> <количество клиентов>
```
