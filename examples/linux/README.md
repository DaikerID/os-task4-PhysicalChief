Собрать с помощью компилятора GCC пример клиент-серверного приложения можно выполнив в консоли следующие команды:
```
gcc client.c -o client
gcc server.c -o server
```
Далее в одном окне консоли необходимо выполнить команду `./server`, а в другом окне командного интерпретатора команду `./client`, чтобы запустить приложение сервера и клиента. Следует обратить внимание, что сервер уже должен быть запущен на момент запуска приложения клиента.

Собрать пример обмена датаграммами по протоколу UDP можно с помощью команд
```
gcc receiver.c -o receiver
gcc sender.c -o sender
```
Для запуска примера необходимо в одном окне командного интерпретатора выполнить `./receiver`, а в другом `./sender`.