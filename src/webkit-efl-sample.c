#include <Ecore.h>
#include <Ecore_Getopt.h>
#include <Ecore_Evas.h>
#include <EWebKit2.h>

#ifndef DEFAULT_URL
#define DEFAULT_URL "http://profusion.mobi/"
#endif

#ifndef DEFAULT_THEME
#define DEFAULT_THEME EWEBKIT2_DATADIR "/themes/default.edj"
#endif

#ifndef DEFAULT_WIDTH
#define DEFAULT_WIDTH 800
#endif

#ifndef DEFAULT_HEIGHT
#define DEFAULT_HEIGHT 600
#endif

#ifndef DEFAULT_EXTENSION_DIR
#define DEFAULT_EXTENSION_DIR "/tmp/"
#endif

#define __STR(x) #x
#define _STR(x) __STR(x)

static void
on_delete(Ecore_Evas *ee EINA_UNUSED)
{
   ecore_main_loop_quit();
}

static void
on_js_alert(Ewk_View_Smart_Data *sd, const char *message)
{
   printf("JS ALERT:\n%s\n\n", message);
}

static void
on_web_view_close(Ewk_View_Smart_Data *sd)
{
   ecore_main_loop_quit();
}

static const char *rotation_choices[] = {
  "0", "90", "180", "270", NULL
};

static const Ecore_Getopt options = {
  "webkit-efl",
  "%prog [options] [url]",
  "1",
  "(C) 2016 ProFUSION Sistemas e Solucoes LTDA\n",
  "Apache-2",
  "Example WebKit-EFL loaded",
  EINA_TRUE,
  {
    ECORE_GETOPT_STORE_STR('e', "engine", "The Ecore-Evas engine (see --list-engines)"),
    ECORE_GETOPT_CALLBACK_NOARGS('E', "list-engines", "List available Ecore-Evas engines",
                                 ecore_getopt_callback_ecore_evas_list_engines, NULL),
    ECORE_GETOPT_STORE_STR(0, "engine-options", "Extra options to pass to engine, like display=XXX;rotation=90"),

    ECORE_GETOPT_STORE_STR('t', "theme", "Path to Edje (*.edj) file with WebKit-EFL theme. Default=" DEFAULT_THEME),

    ECORE_GETOPT_STORE_STR('x', "extension", "Directory where the Webkit extensions are installed. Default=" DEFAULT_EXTENSION_DIR),

    ECORE_GETOPT_CALLBACK_ARGS('g', "geometry", "Specify window geometry. "
                               "Default=0:0:" _STR(DEFAULT_WIDTH) ":" _STR(DEFAULT_HEIGHT),
                               "X:Y:WIDTH:HEIGHT",
                               ecore_getopt_callback_geometry_parse,
                               NULL),
    ECORE_GETOPT_STORE_TRUE('f', "fullscreen", "Start in fullscreen."),
    ECORE_GETOPT_CHOICE('r', "rotation", "Rotation in degrees. If set this will override any values given to --engine-options", rotation_choices),
    ECORE_GETOPT_STORE_TRUE('d', "dev-mode", "This enables the remote webinspector server. To access it, connect to http://localhost:8080"),
    ECORE_GETOPT_VERSION('V', "version"),
    ECORE_GETOPT_COPYRIGHT('C', "copyright"),
    ECORE_GETOPT_HELP('h', "help"),
    ECORE_GETOPT_SENTINEL
  }
};

int
main(int argc, char *argv[])
{
   const char *url = DEFAULT_URL;
   char *engine = NULL;
   char *engine_options = NULL;
   char *theme = DEFAULT_THEME;
   char *extension_dir = DEFAULT_EXTENSION_DIR;
   char *rotation = NULL;
   int args = 1;
   Eina_Rectangle geometry = { 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT };
   Eina_Bool fullscreen = EINA_FALSE;
   Eina_Bool quit_option = EINA_FALSE;
   Eina_Bool dev_mode = EINA_FALSE;
   Ecore_Getopt_Value values[] = {
     ECORE_GETOPT_VALUE_STR(engine),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_STR(engine_options),

     ECORE_GETOPT_VALUE_STR(theme),

     ECORE_GETOPT_VALUE_STR(extension_dir),

     ECORE_GETOPT_VALUE_PTR_CAST(geometry),
     ECORE_GETOPT_VALUE_BOOL(fullscreen),
     ECORE_GETOPT_VALUE_STR(rotation),

     ECORE_GETOPT_VALUE_BOOL(dev_mode),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_BOOL(quit_option),
     ECORE_GETOPT_VALUE_NONE
   };
   Ecore_Evas *ee;
   Evas *evas;
   Ewk_View_Smart_Class sc = EWK_VIEW_SMART_CLASS_INIT_NAME_VERSION("Webkit_Efl_Sample_View");
   Evas_Smart *smart;
   Evas_Object *web_view;
   Ewk_Context *ctx;
   int w, h;
   int ret = EXIT_SUCCESS;
   Ewk_Page_Group *group;

   ecore_init();
   ecore_evas_init();

   ecore_app_args_set(argc, (const char **)argv);
   args = ecore_getopt_parse(&options, values, argc, argv);
   if (args < 0)
     {
        fputs("ERROR: could not parse options.\n", stderr);
        ret = EXIT_FAILURE;
        goto end;
     }

   if (quit_option)
     {
        ret = EXIT_SUCCESS;
        goto end;
     }

   if (args < argc) url = argv[args];

   ee = ecore_evas_new(engine, geometry.x, geometry.y, geometry.w, geometry.h, engine_options);
   if (!ee)
     {
        fprintf(stderr, "ERROR: could not create window. --engine=%s --engine-options=%s\n", engine ? engine : "", engine_options ? engine_options : "");
        goto end;
     }

   evas = ecore_evas_get(ee);

   if (dev_mode)
     putenv("WEBKIT_INSPECTOR_SERVER=127.0.0.1:8080");

   ewk_init();
   ewk_view_smart_class_set(&sc);
   sc.run_javascript_alert = on_js_alert;
   sc.window_close = on_web_view_close;

   ctx = ewk_context_new_with_extensions_path(extension_dir);
   smart = evas_smart_class_new(&sc.sc);
   group = ewk_page_group_create("main");
   if  (dev_mode)
     ewk_settings_developer_extras_enabled_set(ewk_page_group_settings_get(group),
                                               EINA_TRUE);
   web_view = ewk_view_smart_add(evas, smart,
                                 ctx,
                                 group);
   ewk_object_unref(ctx);
   /* query size so rotation from engine_options is automatically managed */
   ecore_evas_geometry_get(ee, NULL, NULL, &w, &h);
   evas_object_resize(web_view, w, h);

   evas_object_show(web_view);
   ecore_evas_object_associate(ee, web_view, ECORE_EVAS_OBJECT_ASSOCIATE_BASE);
   ewk_view_theme_set(web_view, theme);
   ewk_view_url_set(web_view, url);

   ecore_evas_fullscreen_set(ee, fullscreen);
   if (rotation)
     ecore_evas_rotation_set(ee, atoi(rotation));
   ecore_evas_show(ee);
   ecore_evas_callback_delete_request_set(ee, on_delete);

   ecore_evas_focus_set(ee, EINA_TRUE);
   evas_object_focus_set(web_view, EINA_TRUE);

   ecore_main_loop_begin();

   ecore_evas_free(ee);

 end:
   ewk_shutdown();
   ecore_evas_shutdown();
   ecore_shutdown();
   return ret;
}
