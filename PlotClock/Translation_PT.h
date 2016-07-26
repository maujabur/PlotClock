#ifndef TRANSLATION

#define TRANSLATION "PT-BR"

#define CREDITS() {\
    Serial.println(F("Pandora Lab's PlotClock"));\
    Serial.println(F("Codigo original por Johannes Heberlein 2014"));\
    Serial.println(F("Baseado na versao 1.03"));\
    Serial.println();\
    Serial.println(F("Calibracao interativa e modificacoes por "));\
    Serial.println(F("Mau Jabur - maumaker -at- pandoralab -dot- com -dot- br"));\
    Serial.print(F("Versao "));\
    Serial.print(VERSION);\
    Serial.print(F(" "));\
    Serial.print(TRANSLATION);\
    Serial.println();\
    Serial.println(F("Reconhecer comandos pode demorar um pouco."));\
    Serial.println(F("O equipamento precisa terminar as tarefas"));\
  }

#define RTC_OK(){\
    Serial.println(F("DS1307 esta OK."));\
  }
#define RTC_STOPPED(){\
    Serial.println(F("DS1307 esta parado."));\
  }

#define RTC_MISSING(){\
    Serial.println(F("DS1307 nao encontrado."));\
  }

#define OPERATION_MSG() {\
    Serial.println(F("\nOPERACAO"));\
    Serial.println(F("envie [I] para ajuste interativo"));\
  }

#define SET_HOUR_MSG() {\
Serial.println(F("\nAJUSTE DE HORA"));\
  }

#define INTERACTIVE_MSG() {\
    Serial.println();\
    Serial.println(F("AJUSTE INTERATIVO"));\
    Serial.println(F("[N] avanca para o proximo ajuste"));\
    Serial.println(F("[B] volta ao ajuste anterior"));\
    Serial.println(F("[O] modo OPERACAO"));\
    Serial.println(F("[P] salva e retorna a OPERACAO"));\
  }

#define LEFT_X_MSG(){\
    Serial.println(F("\n[W],[S] alinhe o braco ESQUERDO com o eixo X"));\
  }

#define LEFT_Y_MSG(){\
    Serial.println(F("\n[A],[D] alinhe o braco ESQUERDO com o eixo Y"));\
  }

#define RIGHT_X_MSG(){\
    Serial.println(F("\n[W],[S] alinhe o braco DIREITO com o eixo X"));\
  }

#define RIGHT_Y_MSG(){\
    Serial.println(F("\n[A],[D] alinhe o braco DIREITO com o eixo Y"));\
  }

#define PEN_ADJ_MSG(){\
    Serial.println(F("\n[W],[S] ajuste a caneta na superficie"));\
    Serial.println(F("[Q] desenha um teste"));\
  }

#define PARK_ADJ(){\
    Serial.println(F("\n[W],[A],[S],[D] ajuste a caneta no apagador"));\
    Serial.println(F("[Q],[Z] levanta/abaixa caneta"));\
  }

#endif //TRANSLATION
