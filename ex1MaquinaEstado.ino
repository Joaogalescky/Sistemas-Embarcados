/*
Projete uma FSM que gere saída “1” quando o número de uns “1”s na entrada
seja par e maior que zero. Nos demais casos a saída deve ser “0”.

[estado_zero / y = 0] --1--> [estado_impar / y = 0] --1--> [estado_par / y = 1] --1--> [estado_impar / y = 0]
               ^ (0 loop)                    ^ (0 loop)                  ^ (0 loop)
*/

const int LED_PIN = 13;

enum Estados
{
    sZero = 0,
    sImpar = 1,
    sPar = 2
};

Estados estadoAtual = sZero;

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    Serial.begin(9600);
    Serial.println("FSM de Moore - Paridade de 1's (1 quando par e > 0)");
    Serial.println("Envie 0 ou 1 para alimentar a FSM");
    printEstado();
}

int saidaParaEstado(Estados estado)
{
    switch (estado)
    {
    case sZero:
        return 0;
    case sImpar:
        return 0;
    case sPar:
        return 1;
    default:
        return 0; // fallback para evitar warnings
    }
}

void processarEntrada(int x)
{
    Estados proximoEstado = estadoAtual;

    switch (estadoAtual)
    {
    case sZero:
        if (x == 0)
        {
            proximoEstado = sZero;
        }
        else
        {
            proximoEstado = sImpar;
        }
        break;
    case sImpar:
        if (x == 0)
        {
            proximoEstado = sImpar;
        }
        else
        {
            proximoEstado = sPar;
        }
        break;
    case sPar:
        if (x == 0)
        {
            proximoEstado = sPar;
        }
        else
        {
            proximoEstado = sImpar;
        }
        break;
    }

    estadoAtual = proximoEstado;

    digitalWrite(LED_PIN, saidaParaEstado(estadoAtual));

    Serial.print("Entrada: ");
    Serial.print(x);
    Serial.print(" ");
    printEstado();
}

const char *nomeDeEstado(Estados estado)
{
    switch (estado)
    {
    case sZero:
        return "Estado_Zero";
    case sImpar:
        return "Estado_Impar";
    case sPar:
        return "Estado_Par";
    default:
        return "?";
    }
}

void printEstado()
{
    Serial.print("Estado: ");
    Serial.print(nomeDeEstado(estadoAtual));
    Serial.print(" | Saida: ");
    Serial.println(saidaParaEstado(estadoAtual));
}

void loop()
{
    if (Serial.available() > 0)
    {
        char c = Serial.read();
        int x = c - '0';
        if (x == 0 || x == 1)
        {
            processarEntrada(x);
        }
        /*
        Para não passar pelo 'else' desnecessariamente
        Ignora o newline e o carriage return
        */
        else if (c == '\n' || c == '\r')
        {
        }
        else
        {
            Serial.print("(Esperando 0 ou 1) caractere recebido: ");
            Serial.println(c);
        }
    }
}