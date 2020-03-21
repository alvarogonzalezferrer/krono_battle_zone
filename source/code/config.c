//--------------------------------------------------------
// config.c
// Lectura/Escritura de la configuracion del juego
// Esto setea MUCHAS variables globales del juego!
// OJO!!
// (c) 2001, Kronoman
//--------------------------------------------------------

#ifndef CONFIG_C
#define CONFIG_C

//--------------------------------------------------------
// lee la configuracion del archivo
// el archivo debe estar en el path del juego
//--------------------------------------------------------
void leer_configuracion(char *archivo )
{
   set_config_file(archivo);

   // toma la configuracion del disco

   // config video (default: 320, 200)
      H_RES = get_config_int("KRBZ_VIDEO", "H_RES", 320);
      V_RES = get_config_int("KRBZ_VIDEO", "V_RES", 200);
      MODO_VIDEO = GFX_AUTODETECT; // DEFAULT: AUTO-DETECTAR

   // IA
      NIVEL_IA = get_config_int("KRBZ_IA",  "NIVEL_IA", 2);

// detalle grafico
   VER_LUCES   = get_config_int("KRBZ_DETALLE", "VER_LUCES",TRUE);
   VER_PARTI   = get_config_int("KRBZ_DETALLE", "VER_PARTI",TRUE);
   VER_EXPLO   = get_config_int("KRBZ_DETALLE", "VER_EXPLO",TRUE);
   VER_CLIMA   = get_config_int("KRBZ_DETALLE", "VER_CLIMA",TRUE);
   MODO_DE_LUZ = get_config_int("KRBZ_DETALLE", "MODO_DE_LUZ",0);

   SND_DESEO_SONIDO = get_config_int("KRBZ_SONIDO", "DESEO_SONIDO", TRUE);
}

void salvar_configuracion(char *archivo)
{
   set_config_file(archivo);

// salva la configuracion de video
  
  set_config_int("KRBZ_VIDEO", "H_RES", H_RES);
  set_config_int("KRBZ_VIDEO", "V_RES", V_RES);
// IA
  set_config_int("KRBZ_IA","NIVEL_IA",NIVEL_IA);

// detalle grafico
   set_config_int("KRBZ_DETALLE", "VER_LUCES", VER_LUCES);
   set_config_int("KRBZ_DETALLE", "VER_PARTI", VER_PARTI);
   set_config_int("KRBZ_DETALLE", "VER_EXPLO", VER_EXPLO);
   set_config_int("KRBZ_DETALLE", "VER_CLIMA", VER_CLIMA);
   set_config_int("KRBZ_DETALLE", "MODO_DE_LUZ", MODO_DE_LUZ);
// sonido
   set_config_int("KRBZ_SONIDO", "DESEO_SONIDO", SND_DESEO_SONIDO );


}
#endif
