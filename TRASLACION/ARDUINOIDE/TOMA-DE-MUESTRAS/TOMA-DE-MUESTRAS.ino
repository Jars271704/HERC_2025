// Definición de pines
#define AIN1 18  // Control de dirección del motor
#define AIN2 19  // Control de dirección del motor
#define STBY 21  // Pin de standby para habilitar el driver
#define ENCA 34  // Canal A del encoder
#define ENCB 35  // Canal B del encoder
#define POT_PIN 32 // Pin del potenciómetro

// Configuración de la señal PWM
const int freq = 1000;  // Frecuencia de PWM a 1kHz
const int pwmChannel = 15; // Canal PWM asignado
const int resolution = 8; // Resolución de 8 bits (0-255)

// Variables del encoder
volatile long encoderCount = 0; // Contador de pulsos del encoder
const float CPR = 11.0 * 210.0; // Pulsos por revolución ajustados para el motor Ocean 210:1 (11 CPR * 210 reducción)
unsigned long lastTime = 0; // Tiempo de la última medición
int sampleCount = 0;  // Contador de muestras tomadas
const int maxSamples = 300;  // Número máximo de muestras a capturar
const int maxPWM = 240;  // Límite máximo de PWM

int velocidadPWM = 0;  // Inicialización sin voltaje
float voltajeAplicado = 0.0; // Inicialización del voltaje aplicado

// Rutina de interrupción para contar los pulsos del encoder
void IRAM_ATTR encoderISR() {
    if (digitalRead(ENCA) == digitalRead(ENCB)) {
        encoderCount++;
    } else {
        encoderCount--;
    }
}

void setup() {
    Serial.begin(115200);
    
    // Configurar pines como salida o entrada
    pinMode(AIN1, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(STBY, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    pinMode(POT_PIN, INPUT);
    
    // Activar el driver del motor
    digitalWrite(STBY, HIGH);
    
    // Configurar PWM en el ESP32
    ledcAttach(pwmChannel, freq, resolution);
    
    // Configurar la interrupción para el encoder
    attachInterrupt(digitalPinToInterrupt(ENCA), encoderISR, CHANGE);
    
    // Imprimir encabezado de los datos en formato CSV
    Serial.println("Voltaje,VelocidadAngular");
}

// Función para controlar la dirección y velocidad del motor
void motorControl(int velocidad, bool direccion) {
    if (velocidad == 0) {
        // Detener el motor
        digitalWrite(AIN1, LOW);
        digitalWrite(AIN2, LOW);
    } else {
        // Configurar dirección del motor
        digitalWrite(AIN1, direccion);
        digitalWrite(AIN2, !direccion);
    }
    // Aplicar PWM al motor
    ledcWrite(pwmChannel, velocidad);
}

// Función para calcular la velocidad angular del motor
void calcularVelocidad() {
    unsigned long currentTime = millis();
    float deltaTime = (currentTime - lastTime) / 1000.0; // Convertir tiempo a segundos
    lastTime = currentTime;
    
    // Convertir conteo de pulsos a RPM y luego a radianes por segundo
    float rpm = (encoderCount / CPR) * 60.0;
    float velocidadAngular = rpm * (2.0 * PI / 60.0);
    encoderCount = 0; // Reiniciar el contador de pulsos
    
    // Guardar datos en formato CSV
    Serial.print(voltajeAplicado, 2);
    Serial.print(",");
    Serial.println(velocidadAngular, 2);

    // Verificar si se han capturado suficientes muestras o si se ha alcanzado el máximo PWM
    sampleCount++;
    if (sampleCount >= maxSamples || velocidadPWM >= maxPWM) {
        Serial.println("Captura completada. Deteniendo motor.");
        motorControl(0, true); // Detener el motor
        while(true); // Detener la ejecución del programa
    }
}

void loop() {
    // Incrementar progresivamente el voltaje hasta el límite
    if (velocidadPWM < maxPWM) {
        velocidadPWM += 5; // Aumenta la velocidad en cada iteración
    } else {
        velocidadPWM = maxPWM;
    }
    
    // Controlar el motor con el nuevo valor de PWM
    motorControl(velocidadPWM, true);
    
    // Calcular voltaje aplicado al motor
    voltajeAplicado = (velocidadPWM / 255.0) * 12.0;

    delay(100); // Espera 100 ms antes de capturar datos
    calcularVelocidad();
}

