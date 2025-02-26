// Definición de pines para el control del motor y el encoder
#define AIN1 18  // Pin de dirección del motor
#define AIN2 19  // Pin de dirección del motor
#define STBY 21  // Pin de standby para habilitar el driver
#define ENCA 34  // Canal A del encoder
#define ENCB 35  // Canal B del encoder

// Configuración de la señal PWM
const int freq = 1000;  // Frecuencia de PWM (1 kHz)
const int pwmChannel = 15; // Canal PWM asignado
const int resolution = 8; // Resolución de 8 bits (0-255)

// Variables del encoder
volatile long encoderCount = 0; // Contador de pulsos del encoder
const float CPR = 11.0 * 210.0;  // Pulsos por revolución ajustados según la reducción del motor
unsigned long lastTime = 0;  // Último tiempo registrado para calcular la velocidad
float lastError = 0.0;  // Error previo para la acción derivativa
float setpoint = 10.0;  // Velocidad deseada en radianes/segundo
const int maxPWM = 240;  // Límite máximo del valor PWM

// Constantes PID ajustadas
const float Kp = 29226.0284; // Ganancia proporcional
const float Ki = 0.0;  // Ganancia integral (en este caso no se usa)
const float Kd = 12254.6369; // Ganancia derivativa
const float I_max = maxPWM / 2.0;  // Límite del término integral para evitar acumulación excesiva (windup)

// Variables PID
float error, P_term, I_term = 0.0, D_term, output;
float velocidadAngular = 0.0;
float voltajeAplicado = 0.0;

// Variables del filtro de Kalman
float x_est = 0.0;  // Estimación inicial de velocidad
float P_est = 1.0;  // Covarianza inicial
const float Q = 0.01;  // Ruido del proceso
const float R = 0.1;  // Ruido de medición

// Variables para el control del tiempo de muestreo
unsigned long prevTime = 0; // Último tiempo en el que se ejecutó el control
float Ts = 0.01; // Tiempo de muestreo inicial, se actualizará dinámicamente

// Rutina de interrupción para contar los pulsos del encoder
void IRAM_ATTR encoderISR() {
    if (digitalRead(ENCA) == digitalRead(ENCB)) {
        encoderCount++;  // Si los valores de los canales A y B coinciden, el motor gira en una dirección
    } else {
        encoderCount--;  // Si no coinciden, el motor gira en la dirección opuesta
    }
}

void setup() {
    Serial.begin(115200);  // Iniciar comunicación serie para depuración

    // Configurar pines como salidas o entradas
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(STBY, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);

    // Activar el driver del motor
    digitalWrite(STBY, HIGH);

    // Configurar PWM en el ESP32
    ledcAttach(pwmChannel, freq, resolution);

    // Configurar la interrupción para el encoder
    attachInterrupt(digitalPinToInterrupt(ENCA), encoderISR, CHANGE);

    // Imprimir encabezado de los datos
    Serial.println("Voltaje | VelocidadAngular | Error | Output");
}

// Función para controlar la dirección y velocidad del motor
void motorControl(int velocidad, bool direccion) {
    if (velocidad == 0) {  
        // Si la velocidad es 0, detener el motor
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
    } else {  
        // Definir dirección del motor
        digitalWrite(AIN1, direccion);
        digitalWrite(AIN2, !direccion);
    }
    ledcWrite(pwmChannel, velocidad);  // Aplicar el valor de PWM al motor
}

// Función para calcular la velocidad angular con el encoder y aplicar el filtro de Kalman
void calcularVelocidad() {
    unsigned long currentTime = millis(); // Obtener el tiempo actual en milisegundos
    Ts = (currentTime - lastTime) / 1000.0;  // Calcular el tiempo de muestreo en segundos
    lastTime = currentTime; // Actualizar el tiempo anterior

    // Convertir conteo de pulsos a RPM
    float rpm = (encoderCount / CPR) * 60.0;
    float velocidadMedida = rpm * (2.0 * PI / 60.0); // Convertir a radianes por segundo
    encoderCount = 0;  // Reiniciar el contador de pulsos para la siguiente medición

    // Filtro de Kalman
    float x_pred = x_est;  // Predicción de la velocidad
    float P_pred = P_est + Q;  // Predicción de la covarianza

    float K = P_pred / (P_pred + R);  // Cálculo de la ganancia de Kalman
    x_est = x_pred + K * (velocidadMedida - x_pred);  // Actualización de la estimación
    P_est = (1 - K) * P_pred;  // Actualización de la covarianza

    velocidadAngular = x_est;  // Guardar la velocidad filtrada
}

// Función para calcular la acción de control PID
void controlPID() {
    error = setpoint - velocidadAngular;  // Calcular error

    // Calcular términos del PID
    P_term = Kp * error;  // Término proporcional
    I_term += Ki * error * Ts;  // Término integral con factor de muestreo

    // Limitar el término integral para evitar acumulación excesiva (integral windup)
    if (I_term > I_max) I_term = I_max;
    if (I_term < -I_max) I_term = -I_max;

    D_term = Kd * (error - lastError) / Ts;  // Término derivativo
    lastError = error;  // Guardar el error actual para la siguiente iteración

    output = P_term + I_term + D_term;  // Calcular la salida del controlador

    // Limitar la salida dentro del rango permitido
    if (output > maxPWM) output = maxPWM;
    if (output < -maxPWM) output = -maxPWM;

    // Convertir la salida a valor de PWM
    int velocidadPWM = abs(output);
    if (velocidadPWM > maxPWM) velocidadPWM = maxPWM;

    // Controlar la dirección del motor
    if (output >= 0) {
        motorControl(velocidadPWM, true);  // Mover hacia adelante
    } else {
        motorControl(velocidadPWM, false); // Mover hacia atrás
    }

    // Calcular el voltaje aplicado al motor
    voltajeAplicado = (velocidadPWM / 255.0) * 12.0;

    // Imprimir datos para monitoreo
    Serial.print(voltajeAplicado, 2); Serial.print(" | ");
    Serial.print(velocidadAngular, 2); Serial.print(" | ");
    Serial.print(error, 2); Serial.print(" | ");
    Serial.println(output, 2);
}

void loop() {
    unsigned long currentTime = millis();
    if (currentTime - prevTime >= 10) {  // Ejecutar control cada 10 ms
        prevTime = currentTime;
        calcularVelocidad();  // Medir velocidad con el encoder y Kalman
        controlPID();  // Aplicar el control PID
    }
}
