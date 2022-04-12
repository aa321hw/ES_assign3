//Define pins
int SIGNALAPIN = 14;
int SIGNALBPIN = 2;
int analog_input = 4;
int digital_input = 12;

//Define variables used
int error_code = 0;
int average_analogue_in = 0;
int inputvalue = 0;
int signalbdelay = 0.05 * 10;
int digitalval = 0;
int frequency = 0;

long start = 0;
long finish = 0;
long dur = 0 ;

QueueHandle_t queue;


static void Task1(void *argp){

  for (;;){
    
    //Create signal B
    digitalWrite(SIGNALBPIN,HIGH);
    delay(signalbdelay);
    digitalWrite(SIGNALBPIN,LOW);
    vTaskDelay(19/portTICK_PERIOD_MS);
    
  }
  //unsigned stack_hwm = 0, temp;
  
  
  /*
  temp = uxTaskGetStackHighWaterMark(nullptr);
    if ( !stack_hwm || temp < stack_hwm ) {
      stack_hwm = temp;
    }
  Serial.print("Task has stack hwm:");
  Serial.println(stack_hwm);
  */     
}

//Read digital input
static void Task2(void *argp){

  for(;;){

    digitalval = digitalRead(digital_input);
    vTaskDelay(200/portTICK_PERIOD_MS);
    
  } 
}

static void Task3(void *argp){

  for(;;){

    vTaskDelay(1000/portTICK_PERIOD_MS);
    
  } 
}

//Read the analog input and save them into a size 4 array to calculate average
static void Task4(void *argp){

  int current_input = 0;
  for(;;){

    current_input = analogRead(analog_input);
    xQueueSend(queue, &current_input, portMAX_DELAY);
    
    vTaskDelay(41/portTICK_PERIOD_MS);
 
  } 
}

//Calculate the average of the analog input
static void Task5(void *argp){

  for(;;){
    int nval = 0;
    int sum = 0;
    for(int i = 0; i < 4;i++){
      //Serial.println(sum);
      xQueueReceive(queue, &nval, portMAX_DELAY);
      sum = sum + nval;
    }

    average_analogue_in = sum/4;
  
    vTaskDelay(42/portTICK_PERIOD_MS);
    
  } 
  
}

static void Task6(void *argp){

  for(;;){
    
    for (int i = 0; i < 1000; i++){
      __asm__ __volatile__ ("nop");
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
    
  } 
  
  
}
//To make sure that the method works properly, the value used as the half of the maximum value is 2047.5
//Check there is no error
static void Task7(void *argp){

  for(;;){

    if (average_analogue_in > 2047.5){
      error_code = 1;
    }
    
    else{
      error_code = 0;
    }

    vTaskDelay(333/portTICK_PERIOD_MS);
    
  }    
}

//Turn LED on or off depending on error
static void Task8(void *argp){

  for(;;){

    if (error_code == 1){
      digitalWrite(SIGNALAPIN,HIGH);
    }
    
    else{
      digitalWrite(SIGNALAPIN,LOW);
    }
    vTaskDelay(333/portTICK_PERIOD_MS);
    
  } 

}

//Print information obtained from the previous tasks
static void Task9(void *argp){

  
  for(;;){
  
    if (digitalval == 1){
      
      Serial.print("Digital value: ");
      Serial.println(digitalval);
      Serial.print("Frequency: ");
      Serial.println(frequency);
      Serial.print("Average: ");
      Serial.println(average_analogue_in);
      
    }
   
    vTaskDelay(5000/portTICK_PERIOD_MS);
    
  } 
  
}

void setup() {
  
  //Pins setup
  Serial.begin(9600);
  pinMode(SIGNALAPIN, OUTPUT);
  pinMode(SIGNALBPIN,OUTPUT);
  pinMode(analog_input,INPUT);
  pinMode(digital_input,INPUT);

  delay(1000);
  
  int app_cpu = xPortGetCoreID();
  Serial.print(app_cpu);
  Serial.print("here");

  queue = xQueueCreate(4,sizeof(int));
  
  xTaskCreatePinnedToCore(
      Task1,
      "Task1",
      2048,  
      NULL,
      1,
      NULL,
      app_cpu
    );
  /*
  xTaskCreatePinnedToCore(
      Task2,
      "Task2",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task3,
      "Task3",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task4,
      "Task4",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task5,
      "Task5",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task6,
      "Task6",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task7,
      "Task7",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task8,
      "Task8",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );

  xTaskCreatePinnedToCore(
      Task9,
      "Task9",
      2048,  
      nullptr,
      1,
      nullptr,
      app_cpu
    );
  */
}

//Cyclic execute of the tasks
void loop() {

  vTaskDelete(xTaskGetCurrentTaskHandle());
  
   /*
  timer++;
  Task1();
  if (timer % 10 == 0){Task2();}
  if (timer % 50 == 0){Task3();}
  if (timer % 2 == 0){Task4(); Task5();}
  if (timer % 5 == 0){
    Task6();}
  if (timer % 16 == 0){Task7(); Task8();}
  if (timer % 250 == 0){Task9();}
  */
}
