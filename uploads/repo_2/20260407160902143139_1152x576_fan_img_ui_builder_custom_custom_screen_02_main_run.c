#include "custom.h"



static screen_02_main_t *scr = NULL;

//显示时间差值
#if CUSTOM_SCREEN_3_BAKE_LOG
typedef struct {
    int hours;
    int minutes;
    int seconds;
} Time;
// 计算时间差：t1 - t2（用于日志显示）
Time time_subtract(Time t1, Time t2) {
    Time result = {0, 0, 0};

    // 秒的减法
    if (t1.seconds < t2.seconds) {
        t1.minutes -= 1;
        t1.seconds += 60;
    }
    result.seconds = t1.seconds - t2.seconds;

    // 分的减法
    if (t1.minutes < t2.minutes) {
        t1.hours -= 1;
        t1.minutes += 60;
    }
    result.minutes = t1.minutes - t2.minutes;

    // 时的减法
    result.hours = t1.hours - t2.hours;

    return result;
}
#endif


//停止运行的图片变量
bool yk_v_stop_run_state = false;  //false: 暂停  true: 运行


//运行图片数组路径
static const char* food_image_paths_run[YK_FOOD_SUM] = {
    LVGL_IMAGE_PATH(food_00_02.png),
    LVGL_IMAGE_PATH(food_01_02.png),
    LVGL_IMAGE_PATH(food_02_02.png),
    LVGL_IMAGE_PATH(food_03_02.png),
    LVGL_IMAGE_PATH(food_04_02.png),
    LVGL_IMAGE_PATH(food_05_02.png),
    LVGL_IMAGE_PATH(food_06_02.png),
    LVGL_IMAGE_PATH(food_07_02.png),
    LVGL_IMAGE_PATH(food_08_02.png),
    LVGL_IMAGE_PATH(food_09_02.png),
    LVGL_IMAGE_PATH(food_10_02.png),
    LVGL_IMAGE_PATH(food_11_02.png),
    LVGL_IMAGE_PATH(food_12_02.png),
    LVGL_IMAGE_PATH(food_13_02.png),
    LVGL_IMAGE_PATH(food_14_02.png),
    LVGL_IMAGE_PATH(food_15_02.png),
    LVGL_IMAGE_PATH(food_16_02.png),
};


static uint16_t io_count_v = 0;  //发热丝工作一个循环记一次
static uint16_t io_count_var[256][7];  //发热丝开关记一次


#if YK_USE_UI_BUILDER
	
#else
static int32_t ntc_voltage = 4000;//1227短路;//18384;//电压值
// 读取炸蓝插入状态：0 已插入，1 未插入
static uint8_t yk_io_get_fryer()
{
	//printf("获取锅高低电平：Get pin pinvalue=%d\n",pinvalue);
    uint8_t pinvalue=0;  //炸蓝是否插入 0已插入 1没插入
	return pinvalue;
}
// 控制蜂鸣器开关（调试桩/平台实现）
static void yk_io_set_beep(uint8_t pinvalue)
{
	yk_printf("yk_io_set_beep = %d\n",pinvalue);
}
#endif
static bool log_i = 0;  //打印信息到食物名上面 1 打印 0 不打印

static lv_timer_t *task_arc = NULL;  //创建arc任务名
static lv_timer_t *task_io_get = NULL;  //创建io获取状态任务名
static lv_timer_t *task_arc_shake = NULL;  //创建炸蓝抖动任务名



static uint8_t io_get_fryer_i = 0;  //获取是否插入炸锅时过滤波纹
static uint8_t io_get_fryer_i_max = 0;  // 保存io_get_fryer_i的最大值

static uint16_t arc_m = 1;  //倒计时分钟
static int8_t arc_s = 0;  //倒计时秒钟
static uint16_t time_countdown = 100;  //定义倒计时:时间
static uint32_t arc_countdown = 100;  //定义倒计时:arc
static uint32_t arc_countdown_half = 50;  //时间运行到一半，炸蓝抖动图
static uint8_t arc_countdown_half_i = 0;  //时间运行到一半，炸蓝抖动变量
static uint8_t arc_countdown_half_j = 0;  //时间运行到一半，炸蓝抖动次数变量
static bool heating_off = false;  //发热丝开关控制只执行一次 GRF_FALSE关 GRF_TRUE开

static uint16_t food_min = 0;  //开始设置的食物烘烤时间单位 min


static uint16_t ntc_voltage_value[][2] =
{
	#if 0  //NTC电阻100K 3950
	{0,24646},{1,24627},{2,24608},{3,24589},{4,24568},{5,24547},{6,24524},{7,24501},{8,24477},{9,24452},{10,24425},{11,24398},{12,24369},{13,24339},{14,24308},{15,24276},{16,24242},{17,24207},{18,24171},{19,24134},{20,24095},{21,24055},{22,24013},{23,23969},{24,23924},{25,23877},{26,23831},{27,23782},{28,23733},{29,23681},{30,23628},{31,23572},{32,23514},{33,23454},{34,23393},{35,23329},{36,23263},{37,23196},{38,23126},{39,23054},{40,22981},{41,22904},{42,22825},{43,22744},{44,22661},{45,22576},{46,22486},{47,22395},{48,22301},{49,22204},{50,22105},{51,22011},{52,21914},{53,21816},{54,21715},{55,21612},{56,21503},{57,21391},{58,21278},{59,21162},{60,21043},{61,20921},{62,20797},{63,20670},{64,20541},{65,20410},{66,20276},{67,20140},{68,20001},{69,19861},{70,19719},{71,19571},{72,19422},{73,19270},{74,19117},{75,18961},{76,18809},{77,18655},{78,18500},{79,18343},{80,18184},{81,18020},{82,17854},{83,17687},{84,17519},{85,17350},{86,17179},{87,17007},{88,16834},{89,16660},{90,16485},{91,16307},{92,16128},{93,15948},{94,15768},{95,15587},{96,15411},{97,15234},{98,15056},{99,14879},{100,14702},
	{101,14533},{102,14365},{103,14197},{104,14029},{105,13862},{106,13683},{107,13505},{108,13327},{109,13149},{110,12973},{111,12797},{112,12622},{113,12447},{114,12274},{115,12102},{116,11930},{117,11760},{118,11591},{119,11423},{120,11257},{121,11079},{122,10904},{123,10729},{124,10556},{125,10385},{126,10229},{127,10074},{128,9921},{129,9770},{130,9892},{131,9735},{132,9579},{133,9425},{134,9273},{135,9130},{136,8982},{137,8835},{138,8689},{139,8546},{140,8411},{141,8269},{142,8128},{143,7989},{144,7852},{145,7724},{146,7593},{147,7464},{148,7337},{149,7211},{150,7094},{151,6984},{152,6876},{153,6769},{154,6663},{155,6565},{156,6451},{157,6340},{158,6230},{159,6122},{160,6022},{161,5915},{162,5811},{163,5707},{164,5606},{165,5512},{166,5412},{167,5315},{168,5219},{169,5125},{170,4817},{171,4725},{172,4635},{173,4547},{174,4460},{175,4375},{176,4297},{177,4221},{178,4147},{179,4074},{180,4002},{181,3923},{182,3846},{183,3771},{184,3697},{185,3624},{186,3560},{187,3497},{188,3436},{189,3376},{190,3317},{191,3252},{192,3189},{193,3127},{194,3067},{195,3008},{196,2955},{197,2903},{198,2853},{199,2803},{200,2754},
	{201,2711},{202,2668},{203,2627},{204,2586},{205,2546},{206,2501},{207,2456},{208,2413},{209,2371},{210,2329},{211,2287},{212,2245},{213,2205},{214,2165},{215,2126},{216,2088},{217,2050},{218,2013},{219,1977},{220,1942},{221,1908},{222,1874},{223,1842},{224,1809},{225,1778},{226,1747},{227,1717},{228,1687},{229,1659},{230,1630},{231,1602},{232,1575},{233,1548},{234,1522},{235,1496},{236,1471},{237,1447},{238,1422},{239,1398},{240,1375},{241,1352},{242,1330},{243,1308},{244,1287},{245,1265},{246,1245},{247,1225},{248,1204},{249,1185},{250,1166}
	#else  //NTC电阻 STL-5017
	{0,24853},{1,24845},{2,24837},{3,24828},{4,24819},{5,24809},{6,24800},{7,24789},{8,24778},{9,24767},{10,24755},{11,24743},{12,24730},{13,24716},{14,24702},{15,24688},{16,24672},{17,24656},{18,24640},{19,24622},{20,24604},{21,24585},{22,24566},{23,24545},{24,24524},{25,24502},{26,24479},{27,24455},{28,24430},{29,24405},{30,24378},{31,24350},{32,24321},{33,24291},{34,24260},{35,24228},{36,24195},{37,24160},{38,24124},{39,24087},{40,24049},{41,24009},{42,23968},{43,23925},{44,23881},{45,23836},{46,23789},{47,23740},{48,23690},{49,23638},{50,23585},{51,23529},{52,23472},{53,23414},{54,23354},{55,23291},{56,23227},{57,23161},{58,23094},{59,23024},{60,22953},{61,22879},{62,22803},{63,22726},{64,22646},{65,22565},{66,22481},{67,22396},{68,22308},{69,22218},{70,22126},{71,22031},{72,21935},{73,21837},{74,21736},{75,21633},{76,21528},{77,21421},{78,21311},{79,21200},{80,21086},{81,20970},{82,20852},{83,20732},{84,20610},{85,20486},{86,20359},{87,20231},{88,20100},{89,19967},{90,19833},{91,19696},{92,19558},{93,19417},{94,19275},{95,19131},{96,18985},{97,18837},{98,18688},{99,18537},{100,18384},
	{101,18230},{102,18074},{103,17917},{104,17758},{105,17598},{106,17437},{107,17275},{108,17111},{109,16947},{110,16780},{111,16614},{112,16446},{113,16278},{114,16108},{115,15938},{116,15767},{117,15596},{118,15424},{119,15252},{120,15079},{121,14906},{122,14733},{123,14559},{124,14385},{125,14212},{126,14039},{127,13864},{128,13692},{129,13518},{130,13345},{131,13173},{132,13000},{133,12828},{134,12657},{135,12488},{136,12317},{137,12148},{138,11979},{139,11811},{140,11644},{141,11478},{142,11313},{143,11150},{144,10986},{145,10824},{146,10663},{147,10504},{148,10345},{149,10188},{150,10014},{151,9862},{152,9712},{153,9563},{154,9418},{155,9272},{156,9128},{157,8985},{158,8844},{159,8705},{160,8568},{161,8432},{162,8297},{163,8166},{164,8034},{165,7906},{166,7778},{167,7651},{168,7527},{169,7404},{170,7282},{171,7164},{172,7047},{173,6931},{174,6816},{175,6703},{176,6591},{177,6481},{178,6372},{179,6268},{180,6163},{181,6060},{182,5959},{183,5860},{184,5759},{185,5664},{186,5568},{187,5475},{188,5380},{189,5291},{190,5202},{191,5115},{192,5027},{193,4941},{194,4859},{195,4776},{196,4695},{197,4614},{198,4536},{199,4461},{200,4385},
	{201,4313},{202,4240},{203,4166},{204,4096},{205,4029},{206,3961},{207,3893},{208,3828},{209,3763},{210,3698},{211,3636},{212,3577},{213,3515},{214,3456},{215,3400},{216,3341},{217,3284},{218,3232},{219,3176},{220,3123},{221,3074},{222,3020},{223,2971},{224,2921},{225,2876},{226,2826},{227,2779},{228,2733},{229,2691},{230,2644},{231,2601},{232,2559},{233,2516},{234,2477},{235,2434},{236,2395},{237,2355},{238,2320},{239,2281},{240,2246},{241,2211},{242,2175},{243,2140},{244,2104},{245,2073},{246,2037},{247,2005},{248,1974},{249,1942},{250,1910},
	{251,1810},{252,1783},{253,1751},{254,1723},{255,1696},{256,1668},{257,1640},{258,1612},{259,1589},{260,1556},{261,1533},{262,1510},{263,1485},{264,1462},{265,1438},{266,1415},{267,1387},{268,1372},{269,1349},{270,1330},{271,1306},{272,1287},{273,1267},{274,1248},{275,1224},{276,1205},{277,1186},{278,1171},{279,1152},{280,1132},{281,1118},{282,1098},{283,1083},{284,1064},{285,1050},{286,1030},{287,1016},{288,1001},{289,986},{290,971},{291,951},{292,941},{293,927},{294,912},{295,897},{296,883},{297,873},{298,862},{299,849},{300,833}
	#endif
};


static int8_t ntc_t_difference = 0;  //调节温度差值


// 调试温度差值函数：根据设定温度给出 NTC 温差补偿
static void func_t_mft(void)
{
	#if P_TEST_MODE == 0  //0:572(AC) 1：6001(AC) 2:AB800C(DC)

		#if YK_US_ENGLISH == 1
		if (yk_v_food_t_f >= 30 && yk_v_food_t_f <= 35)  //32(-6) 35(-6)
		{
			ntc_t_difference = -18;
		}
		else if (yk_v_food_t_f > 35 && yk_v_food_t_f <= 50)  //40(-7)
		{
			ntc_t_difference = -18;
		}
		else if (yk_v_food_t_f > 50 && yk_v_food_t_f <= 55)  //48(-7) 50(-7)
		{
			ntc_t_difference = -11;  //STL
		}
		else if (yk_v_food_t_f > 55 && yk_v_food_t_f <= 70)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -17;  //STL
		}
		else if (yk_v_food_t_f > 70 && yk_v_food_t_f <= 75)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -11;  //STL
		}
		else if (yk_v_food_t_f > 75 && yk_v_food_t_f <= 80)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -11;  //STL
		}
		else if (yk_v_food_t_f > 80 && yk_v_food_t_f <= 85)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -9;  //STL
		}
		else if (yk_v_food_t_f > 85 && yk_v_food_t_f <= 90)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -9;  //STL
		}
		else if (yk_v_food_t_f > 90 && yk_v_food_t_f <= 95)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -9;  //STL
		}
		else if (yk_v_food_t_f > 95 && yk_v_food_t_f <= 100)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -8;  //STL
		}
		else if (yk_v_food_t_f > 100 && yk_v_food_t_f <= 105)  //100(-8),101(-8),105(-8),111(-8)
		{
			ntc_t_difference = -7;  //STL
		}
		else if (yk_v_food_t_f > 105 && yk_v_food_t_f <= 110)  //100(-8),101(-8),105(-8),111(-8)
		{
			ntc_t_difference = -7;  //STL
		}
		else if (yk_v_food_t_f > 110 && yk_v_food_t_f <= 120)  //100(-8),101(-8),105(-8),111(-8)
		{
			ntc_t_difference = -7;  //STL
		}
		else if (yk_v_food_t_f > 120 && yk_v_food_t_f <= 125)  //115(-7)
		{
			ntc_t_difference = -6;  //STL
		}
		else if (yk_v_food_t_f > 125 && yk_v_food_t_f <= 130)  //101(-8),121(-8)
		{ 
			ntc_t_difference = -6;   //STL
		}
		else if (yk_v_food_t_f > 130 && yk_v_food_t_f <= 135)  //140(-4)
		{
			ntc_t_difference = -5;  //STL
		}
		else if (yk_v_food_t_f > 135 && yk_v_food_t_f <= 140)  //140(-4)
		{
			ntc_t_difference = -3;  //STL
		}
		else if (yk_v_food_t_f > 140 && yk_v_food_t_f <= 145)  //145(-3)
		{
			ntc_t_difference = -1; //STL
		}
		else if (yk_v_food_t_f > 145 && yk_v_food_t_f <= 150)  //150(-2)
		{
			ntc_t_difference = 1;  //STL
		}
		else if (yk_v_food_t_f > 150 && yk_v_food_t_f <= 155)  //155(0)
		{
			ntc_t_difference = 3;  //STL
		}
		else if (yk_v_food_t_f > 155 && yk_v_food_t_f <= 160)  //160(2)
		{
			ntc_t_difference = 5;  //STL
		}
		else if (yk_v_food_t_f > 160 && yk_v_food_t_f <= 165)  //165(3)
		{
			ntc_t_difference = 6;  //STL
		}
		else if (yk_v_food_t_f > 165 && yk_v_food_t_f <= 170)  //170(3)
		{
			ntc_t_difference = 8;  //STL
		}
		else if (yk_v_food_t_f > 170 && yk_v_food_t_f <= 175)  //175(5)
		{
			ntc_t_difference = 9;  //STL
		}
		else if (yk_v_food_t_f > 175 && yk_v_food_t_f <= 180)  //180(7)
		{
			ntc_t_difference = 10;  //STL
		}
		else if (yk_v_food_t_f > 180 && yk_v_food_t_f <= 185)  //185(9)
		{
			ntc_t_difference = 11;  //STL
		}
		else if (yk_v_food_t_f > 185 && yk_v_food_t_f <= 190)  //190(11)
		{
			ntc_t_difference = 12;  //STL
		}
		else if (yk_v_food_t_f > 190 && yk_v_food_t_f <= 195)  //195(13)
		{
			ntc_t_difference = 13;  //-7;  //STL
		}
		else if (yk_v_food_t_f > 195 && yk_v_food_t_f <= 200)  //200(15)
		{
			ntc_t_difference = 17;  //STL：15
		}
		#else
		if (yk_v_food_t_f >= 30 && yk_v_food_t_f <= 35)  //32(-6) 35(-6)
		{
			ntc_t_difference = -15;  //-18;
		}
		else if (yk_v_food_t_f > 35 && yk_v_food_t_f <= 50)  //40(-7)
		{
			ntc_t_difference = -16;  //-18;
		}
		else if (yk_v_food_t_f > 50 && yk_v_food_t_f <= 55)  //48(-7) 50(-7)
		{
			ntc_t_difference = -17;  //-18;  //STL
		}
		else if (yk_v_food_t_f > 55 && yk_v_food_t_f <= 60)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -18;  //-19;  //STL
		}
		else if (yk_v_food_t_f > 60 && yk_v_food_t_f <= 65)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -19;  //-19;  //STL
		}
		else if (yk_v_food_t_f > 65 && yk_v_food_t_f <= 70)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -20;  //-19;  //STL
		}
		else if (yk_v_food_t_f > 70 && yk_v_food_t_f <= 75)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -21;  //-18;  //STL
		}
		else if (yk_v_food_t_f > 75 && yk_v_food_t_f <= 80)  //55(-8) 60(-8) 68(-8) 70(-8)
		{
			ntc_t_difference = -22;  //-18;  //STL
		}
		else if (yk_v_food_t_f > 80 && yk_v_food_t_f <= 85)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -23;  //-17;  //STL
		}
		else if (yk_v_food_t_f > 85 && yk_v_food_t_f <= 90)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -24;  //-17;  //STL
		}
		else if (yk_v_food_t_f > 90 && yk_v_food_t_f <= 95)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -25;  //-17;  //STL
		}
		else if (yk_v_food_t_f > 95 && yk_v_food_t_f <= 100)  //80(-8) 82(-8) 90(-8)
		{
			ntc_t_difference = -26;  //-16;  //STL
		}
		else if (yk_v_food_t_f > 100 && yk_v_food_t_f <= 105)  //100(-8),101(-8),105(-8),111(-8)
		{
			ntc_t_difference = -27;  //-15;  //STL
		}
		else if (yk_v_food_t_f > 105 && yk_v_food_t_f <= 110)  //100(-8),101(-8),105(-8),111(-8)
		{
			ntc_t_difference = -28;  //-15;  //STL
		}
		else if (yk_v_food_t_f > 110 && yk_v_food_t_f <= 120)  //100(-8),101(-8),105(-8),111(-8)
		{
			ntc_t_difference = -28;  //-15;  //STL
		}
		else if (yk_v_food_t_f > 120 && yk_v_food_t_f <= 125)  //115(-7)
		{
			ntc_t_difference = -28;  //-14;  //STL
		}
		else if (yk_v_food_t_f > 125 && yk_v_food_t_f <= 130)  //101(-8),121(-8)
		{ 
			ntc_t_difference = -26;  //-13;   //STL
		}
		else if (yk_v_food_t_f > 130 && yk_v_food_t_f <= 135)  //140(-4)
		{
			ntc_t_difference = -26;  //-11;  //STL
		}
		else if (yk_v_food_t_f > 135 && yk_v_food_t_f <= 140)  //140(-4)
		{
			ntc_t_difference = -24;  //-10;  //STL
		}
		else if (yk_v_food_t_f > 140 && yk_v_food_t_f <= 145)  //145(-3)
		{
			ntc_t_difference = -24;  //-8; //STL
		}
		else if (yk_v_food_t_f > 145 && yk_v_food_t_f <= 150)  //150(-2)
		{
			ntc_t_difference = -22;  //-6;  //STL
		}
		else if (yk_v_food_t_f > 150 && yk_v_food_t_f <= 155)  //155(0)
		{
			ntc_t_difference = -22;  //-5;  //STL
		}
		else if (yk_v_food_t_f > 155 && yk_v_food_t_f <= 160)  //160(2)
		{
			ntc_t_difference = -20;  //-3;  //STL
		}
		else if (yk_v_food_t_f > 160 && yk_v_food_t_f <= 165)  //165(3)
		{
			ntc_t_difference = -20;  //-2;  //STL
		}
		else if (yk_v_food_t_f > 165 && yk_v_food_t_f <= 170)  //170(3)
		{
			ntc_t_difference = -19;  //0;  //STL
		}
		else if (yk_v_food_t_f > 170 && yk_v_food_t_f <= 175)  //175(5)
		{
			ntc_t_difference = -19;  //1;  //STL
		}
		else if (yk_v_food_t_f > 175 && yk_v_food_t_f <= 180)  //180(7)
		{
			ntc_t_difference = -18;  //2;  //STL
		}
		else if (yk_v_food_t_f > 180 && yk_v_food_t_f <= 185)  //185(9)
		{
			ntc_t_difference = -18;  //3;  //STL
		}
		else if (yk_v_food_t_f > 185 && yk_v_food_t_f <= 190)  //190(11)
		{
			ntc_t_difference = -17;  //4;  //STL
		}
		else if (yk_v_food_t_f > 190 && yk_v_food_t_f <= 195)  //195(13)
		{
			ntc_t_difference = -17;  //5;  //-7;  //STL
		}
		else if (yk_v_food_t_f > 195 && yk_v_food_t_f <= 200)  //200(15)
		{
			ntc_t_difference = -15;  //9;  //STL：15
		}
		#endif

	#elif P_TEST_MODE == 1
	if (yk_v_food_t_f >= 30 && yk_v_food_t_f <= 35)  //32(-6) 35(-6)
	{
		ntc_t_difference = -6;
	}
	else if (yk_v_food_t_f > 35 && yk_v_food_t_f <= 40)  //40(-7)
	{
		ntc_t_difference = -7;
	}
	else if (yk_v_food_t_f > 40 && yk_v_food_t_f <= 50)  //48(-7) 50(-7)
	{
		ntc_t_difference = -11;  //STL
	}
	else if (yk_v_food_t_f > 50 && yk_v_food_t_f <= 75)  //55(-8) 60(-8) 68(-8) 70(-8)
	{
		ntc_t_difference = -6;  //STL
	}
	else if (yk_v_food_t_f > 75 && yk_v_food_t_f <= 100)  //80(-8) 82(-8) 90(-8)
	{
		ntc_t_difference = -4;  //STL
	}
	else if (yk_v_food_t_f > 100 && yk_v_food_t_f <= 110)  //100(-8),101(-8),105(-8),111(-8)
	{
		ntc_t_difference = -6;  //STL
	}
	else if (yk_v_food_t_f > 110 && yk_v_food_t_f <= 125)  //115(-7)
	{
		ntc_t_difference = -4;  //STL
	}
	else if (yk_v_food_t_f > 125 && yk_v_food_t_f <= 130)  //101(-8),121(-8)
	{ 
		ntc_t_difference = -3;   //STL
	}
	else if (yk_v_food_t_f > 130 && yk_v_food_t_f <= 140)  //140(-4)
	{
		ntc_t_difference = -2;  //STL
	}
	else if (yk_v_food_t_f > 140 && yk_v_food_t_f <= 145)  //145(-3)
	{
		ntc_t_difference = -1; //STL
	}
	else if (yk_v_food_t_f > 145 && yk_v_food_t_f <= 150)  //150(-2)
	{
		ntc_t_difference = 0;  //STL
	}
	else if (yk_v_food_t_f > 150 && yk_v_food_t_f <= 155)  //155(0)
	{
		ntc_t_difference = 0;  //STL
	}
	else if (yk_v_food_t_f > 155 && yk_v_food_t_f <= 160)  //160(2)
	{
		ntc_t_difference = 1;  //STL
	}
	else if (yk_v_food_t_f > 160 && yk_v_food_t_f <= 165)  //165(3)
	{
		ntc_t_difference = 2;  //STL
	}
	else if (yk_v_food_t_f > 165 && yk_v_food_t_f <= 170)  //170(3)
	{
		ntc_t_difference = 4;  //STL
	}
	else if (yk_v_food_t_f > 170 && yk_v_food_t_f <= 175)  //175(5)
	{
		ntc_t_difference = 5;  //STL
	}
	else if (yk_v_food_t_f > 175 && yk_v_food_t_f <= 180)  //180(7)
	{
		ntc_t_difference = 6;  //STL
	}
	else if (yk_v_food_t_f > 180 && yk_v_food_t_f <= 185)  //185(9)
	{
		ntc_t_difference = 7;  //STL
	}
	else if (yk_v_food_t_f > 185 && yk_v_food_t_f <= 190)  //190(11)
	{
		ntc_t_difference = 7;  //STL
	}
	else if (yk_v_food_t_f > 190 && yk_v_food_t_f <= 195)  //195(13)
	{
		ntc_t_difference = 11;  //-7;  //STL
	}
	else if (yk_v_food_t_f > 195 && yk_v_food_t_f <= 200)  //200(15)
	{
		ntc_t_difference = 27;  //13;  //-8  6;  //15;  //STL
	}
	#elif P_TEST_MODE == 2
	if (yk_v_food_t_f >= 30 && yk_v_food_t_f <= 35)  //32(-6) 35(-6)
	{
		ntc_t_difference = -4;
	}
	else if (yk_v_food_t_f > 35 && yk_v_food_t_f <= 40)  //40(-7)
	{
		ntc_t_difference = -4;
	}
	else if (yk_v_food_t_f > 40 && yk_v_food_t_f <= 50)  //48(-7) 50(-7)
	{
		ntc_t_difference = -4;  //STL
	}
	else if (yk_v_food_t_f > 50 && yk_v_food_t_f <= 75)  //55(-8) 60(-8) 68(-8) 70(-8)
	{
		ntc_t_difference = -4;  //STL
	}
	else if (yk_v_food_t_f > 75 && yk_v_food_t_f <= 100)  //80(-8) 82(-8) 90(-8)
	{
		ntc_t_difference = -5;  //STL
	}
	else if (yk_v_food_t_f > 100 && yk_v_food_t_f <= 110)  //100(-8),101(-8),105(-8),111(-8)
	{
		ntc_t_difference = -5;  //STL
	}
	else if (yk_v_food_t_f > 110 && yk_v_food_t_f <= 125)  //115(-7)
	{
		ntc_t_difference = -4;  //STL
	}
	else if (yk_v_food_t_f > 125 && yk_v_food_t_f <= 130)  //101(-8),121(-8)
	{ 
		ntc_t_difference = -3;   //STL
	}
	else if (yk_v_food_t_f > 130 && yk_v_food_t_f <= 140)  //140(-4)
	{
		ntc_t_difference = -3;  //STL
	}
	else if (yk_v_food_t_f > 140 && yk_v_food_t_f <= 145)  //145(-3)
	{
		ntc_t_difference = -3; //STL
	}
	else if (yk_v_food_t_f > 145 && yk_v_food_t_f <= 150)  //150(-2)
	{
		ntc_t_difference = 0;  //STL
	}
	else if (yk_v_food_t_f > 150 && yk_v_food_t_f <= 155)  //155(0)
	{
		ntc_t_difference = 3;  //STL
	}
	else if (yk_v_food_t_f > 155 && yk_v_food_t_f <= 160)  //160(2)
	{
		ntc_t_difference = 5;  //STL
	}
	else if (yk_v_food_t_f > 160 && yk_v_food_t_f <= 165)  //165(3)
	{
		ntc_t_difference = 7;  //STL
	}
	else if (yk_v_food_t_f > 165 && yk_v_food_t_f <= 170)  //170(3)
	{
		ntc_t_difference = 8;  //STL
	}
	else if (yk_v_food_t_f > 170 && yk_v_food_t_f <= 175)  //175(5)
	{
		ntc_t_difference = 10;  //STL
	}
	else if (yk_v_food_t_f > 175 && yk_v_food_t_f <= 180)  //180(7)
	{
		ntc_t_difference = 11;  //STL
	}
	else if (yk_v_food_t_f > 180 && yk_v_food_t_f <= 185)  //185(9)
	{
		ntc_t_difference = 14;  //STL
	}
	else if (yk_v_food_t_f > 185 && yk_v_food_t_f <= 190)  //190(11)
	{
		ntc_t_difference = 15;  //STL
	}
	else if (yk_v_food_t_f > 190 && yk_v_food_t_f <= 195)  //195(13)
	{
		ntc_t_difference = 16;  //-7;  //STL
	}
	else if (yk_v_food_t_f > 195 && yk_v_food_t_f <= 200)  //200(15)
	{
		ntc_t_difference = 25;  //13;  //-8  6;  //15;  //STL
	}
	#endif

	// if (yk_v_food_t_f >= 150)
	// {
	// 	ntc_t_difference -= 30;
	// }

	#if P_TEST_MODE_CZ == 1  //5寸参展用的怕溶胶 大于150度的时候都减15度
		if (yk_v_food_t_f >= 150)
		{
			ntc_t_difference -= 15;
		}
	#endif
    yk_printf("ntc_t_difference:%d\r\n", ntc_t_difference);
}

void task_del(void);
static void task_create_io_get(void);
//删除获取炸蓝插入任务
static void task_del_io_get();
static void task_create(uint16_t food_min_value);


// 发热丝关（避免重复下发）
static void yk_io_set_heating_off()  //发热丝关
{
	if (heating_off)
	{
		printf("yk_io_set_heating_off--\n");
		#if YK_USE_UI_BUILDER
		//发热丝关
		yk_io_set_heating(0);
		yk_hrtimer_close(YK_HRTIMER_HEATING_CH);
		yk_printf("ntc_voltage发热丝关 == %d\n",ntc_voltage);
		#endif

		#if CUSTOM_SCREEN_3_BAKE_LOG
		io_count_var[io_count_v][0]++;
		yk_printf("io_count_voff:%d %02d:%02d:%02d \n",io_count_var[io_count_v][0],yk_v_hor_value,yk_v_min_value,yk_v_sec_value);
		#endif

		heating_off = false;
	}
}
// 发热丝开（避免重复下发）
static void yk_io_set_heating_on()  //发热丝开
{
	if (heating_off == false)
	{
		printf("yk_io_set_heating_on--\n");
		#if YK_USE_UI_BUILDER
		//发热丝开
		yk_io_set_heating(1);
		yk_hrtimer_open(YK_HRTIMER_HEATING_CH);
		yk_printf("ntc_voltage发热丝开 == %d\n",ntc_voltage);
		#endif

		heating_off = true;
	}
}


// 是否禁用控件：运行时禁用滑块，暂停/取消后恢复
static void func_disable_slider()
{
	if (yk_v_stop_run_state)
	{
        // 运行中：把温度/时间标题变灰，提示当前不可调
        lv_obj_set_style_text_color(scr->label_5_t, lv_color_hex(0x919393), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(scr->label_6_time, lv_color_hex(0x919393), LV_PART_MAIN | LV_STATE_DEFAULT);

        // 运行中：隐藏滑块最左侧圆点，避免误以为可拖动
        lv_obj_add_flag(scr->image_1_28_28, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(scr->image_2_28_28, LV_OBJ_FLAG_HIDDEN);

        // 运行中：禁用滑块，防止中途改温度/时间
        lv_obj_add_state(scr->slider_1_t, LV_STATE_DISABLED);
        lv_obj_add_state(scr->slider_2_time, LV_STATE_DISABLED);

        // 运行中：禁止切换四个 DIY 模式，避免运行参数被中途改动
        // 注意：这里只加 LV_STATE_DISABLED，不动任何透明度；灰化效果在主界面创建时通过“禁用态仅改颜色”的样式实现
        lv_obj_add_state(scr->label_1_airfry, LV_STATE_DISABLED);
        lv_obj_add_state(scr->label_2_bake, LV_STATE_DISABLED);
        lv_obj_add_state(scr->label_3_preheat, LV_STATE_DISABLED);
        lv_obj_add_state(scr->label_4_dehydrate, LV_STATE_DISABLED);
	}
	else
	{
        // 停止/暂停后：恢复温度/时间标题颜色
        lv_obj_set_style_text_color(scr->label_5_t, lv_color_hex(0xe47a46), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_color(scr->label_6_time, lv_color_hex(0xe47a46), LV_PART_MAIN | LV_STATE_DEFAULT);

        // 停止/暂停后：恢复滑块圆点显示
        lv_obj_clear_flag(scr->image_1_28_28, LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(scr->image_2_28_28, LV_OBJ_FLAG_HIDDEN);

        // 停止/暂停后：启用滑块，允许调整温度/时间
        lv_obj_clear_state(scr->slider_1_t, LV_STATE_DISABLED);
        lv_obj_clear_state(scr->slider_2_time, LV_STATE_DISABLED);

        // 停止/暂停后：恢复四个 DIY 模式可点击
        lv_obj_clear_state(scr->label_1_airfry, LV_STATE_DISABLED);
        lv_obj_clear_state(scr->label_2_bake, LV_STATE_DISABLED);
        lv_obj_clear_state(scr->label_3_preheat, LV_STATE_DISABLED);
        lv_obj_clear_state(scr->label_4_dehydrate, LV_STATE_DISABLED);
	}
}


static lv_timer_t *task_beep = NULL;  //创建蜂鸣器任务名
static uint8_t task_beep_i = 0;  //蜂鸣器任务计数器
// 删除蜂鸣器任务并关闭蜂鸣器
static void task_del_beep()
{
	if (task_beep != NULL) {
		yk_io_set_beep(0);
		lv_timer_del(task_beep);
		task_beep = NULL;
	}
}
// 蜂鸣器节拍回调：按计数器输出固定鸣叫序列
static void task_beep_cd()
{
	yk_printf("task_beep_cd=================\n");
	task_beep_i++;
	if (task_beep_i == 1)
	{
		yk_io_set_beep(1);
	}
	else if (task_beep_i == 2)
	{
		yk_io_set_beep(0);
	}
	else if (task_beep_i == 3)
	{
		yk_io_set_beep(1);
	}
	else if (task_beep_i == 4)
	{
		yk_io_set_beep(0);
	}
	else if (task_beep_i == 5)
	{
		yk_io_set_beep(1);
	}
	else if (task_beep_i == 6)
	{
		yk_io_set_beep(0);
	}
	else if (task_beep_i == 7)
	{
		yk_io_set_beep(1);
	}
	else if (task_beep_i == 8)
	{
		yk_io_set_beep(0);
	}
	else if (task_beep_i == 9)
	{
		yk_io_set_beep(1);
	}
	else if (task_beep_i == 10)
	{
		task_beep_i = 0;
		task_del_beep();
	}
	yk_printf("task_beep_i ======================== %d \n",task_beep_i);
}

// 创建蜂鸣器任务：按 period 周期触发节拍回调
static void task_create_beep(uint32_t period)
{
	if(task_beep == NULL)task_beep = lv_timer_create(task_beep_cd,period,NULL);
	lv_timer_ready(task_beep);
}

//创建风扇延迟关闭控件名
static lv_timer_t *timer_fan_off = NULL;

// 关闭风扇回调函数：到期后关风扇并清理定时器
static void timer_fan_off_cb()
{
	if (timer_fan_off)
	{
		lv_timer_del(timer_fan_off);
		timer_fan_off = NULL;
	}
	#if YK_USE_UI_BUILDER
		yk_hrtimer_close(YK_HRTIMER_FAN_CH);
		yk_io_set_fan(0);
	#endif
}

// 开启风扇：若存在“延迟关风扇”定时器则先取消
static void task_create_fan_on()
{
	if(timer_fan_off) 
	{
		lv_timer_del(timer_fan_off);
		timer_fan_off = NULL;
	}
	#if YK_USE_UI_BUILDER
	yk_io_set_fan(1);
	yk_hrtimer_open(YK_HRTIMER_FAN_CH);
	#endif
}
// 创建风扇延迟关闭任务
static void task_create_fan_off()
{
	if(timer_fan_off) 
	{
		lv_timer_del(timer_fan_off);
		timer_fan_off = NULL;
	} 
	if(!timer_fan_off) timer_fan_off = lv_timer_create(timer_fan_off_cb, 60000*2, NULL);
}

// 取消运行：停止任务、关加热/风扇、恢复主界面控件
static void yk_f_cancel(void)
{ 
    yk_printf("yk_f_cancel");

    screensaver_time = 600*2;  //2分钟进入屏保时钟界面
	//删除arc倒计时任务、删除获取炸蓝插入任务、删除获取电压值任务
    task_del();
	//创建蜂鸣器任务
	if (yk_v_stop_run_state) {
		//蜂鸣器响
		task_create_beep(500);
	}
	//风扇延迟关
	task_create_fan_off();
	//发热丝关
	yk_io_set_heating_off();
	//倒计时很短时需要这个,删除运行到一半蜂鸣器任务
	if (task_arc_shake) {
		lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		arc_countdown_half_i = 0;
		arc_countdown_half_j = 0;
		lv_timer_del(task_arc_shake);
		task_arc_shake = NULL;
	}


	#if P_AGING_TEST  //更新到退出时间、保存次数、创建倒计时回调函数30分钟后在进入烘烤界面进行老化测试
	lv_label_set_text_fmt(label_aging_test, "%02d:%02d\n aging_test_count: %d",yk_v_hor_value,yk_v_min_value,yk_settings.aging_test_count);
	yk_f_file_flash_save(0, yk_settings.aging_test_count++);
	lv_timer_create(aging_test_cb, 1000*60*30, NULL);
	#endif


    //显示运行容器
    lv_obj_add_flag(scr->container_2_3_run, LV_OBJ_FLAG_HIDDEN);
    //判断是亮色还是暗色模式
	if (yk_settings.display_mode == val1_display_mode_light)
	{
		lv_img_set_src(scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_0.png));
	}
	else if (yk_settings.display_mode == val1_display_mode_dark)
	{
		lv_img_set_src(scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_0_dark.png));
	}
    yk_v_stop_run_state = false;
    //启用控件
    func_disable_slider();
    // //显示控件
    // lv_obj_clear_flag(scr->label_1_airfry, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(scr->label_2_bake, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(scr->label_3_preheat, LV_OBJ_FLAG_HIDDEN);
    // lv_obj_clear_flag(scr->label_4_dehydrate, LV_OBJ_FLAG_HIDDEN);
}

// 点击开始/暂停：state=true 表示手动点击，false 表示弹框触发
void yk_f_run_start_pause_clicked(bool  state)
{
    yk_printf("yk_f_run_start_pause_clicked state == %d\n",state);
    if(yk_v_stop_run_state)
    {
        // 当前为“运行中”状态：切换为暂停
        yk_v_stop_run_state = false;
		//暂停圆弧倒计时任务
        if (task_arc)lv_timer_pause(task_arc);
		//判断是亮色还是暗色模式
		if (yk_settings.display_mode == val1_display_mode_light)
		{
			lv_img_set_src(scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_0.png));
		}
		else if (yk_settings.display_mode == val1_display_mode_dark)
		{
			lv_img_set_src(scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_stop_0_dark.png));
		}

		#if YK_USE_UI_BUILDER
		//删除获取电压值任务
		yk_f_psadc_del();
		#endif
		//发热丝关
		yk_io_set_heating_off();
		//风扇延迟关
		task_create_fan_off();
		//判断是弹框,还是手动点击，如果是手动点击这个值等于224，如果是弹框这个值等于0，手动点击为真进入删除获取炸蓝任务
		if (state) {
			//删除获取炸蓝插入任务
			task_del_io_get();
		}

		//倒计时很短时需要这个,删除运行到一半蜂鸣器任务
		if (task_arc_shake) {
			//隐藏抖动文字显示
			lv_obj_clear_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
			arc_countdown_half_i = 0;
			arc_countdown_half_j = 0;
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(0);
			#endif
			lv_timer_del(task_arc_shake);
			task_arc_shake = NULL;
			lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
    }else{
        // 当前为“暂停/未运行”状态：切换为运行
        yk_v_stop_run_state = true;

		//判断是否重新创建arc倒计时任务
		if (food_min != yk_v_food_min)
		{
			task_del();
            //立即刷新倒计时时间
	        lv_label_set_text_fmt(scr->label_run_timer, "%02d:%02d",yk_v_food_min,0);
			//创建时间和圆弧倒计时任务
			if(task_arc == NULL)task_create(yk_v_food_min);
		}else
		{
            if(task_arc == NULL)
            {
                task_create(yk_v_food_min);
            }
            else
            {
               if(task_arc)lv_timer_resume(task_arc);
            }
		}

		
		lv_img_set_src(scr->image_7_stop_run, LVGL_IMAGE_PATH(icon_run_1.png));
		#if YK_USE_UI_BUILDER
		//创建获取电压值任务
		yk_f_psadc_create();
		#endif
		//发热丝开
		yk_io_set_heating_on();
		//风扇开
		task_create_fan_on();
		//判断不同温度调差值
		func_t_mft();
		//创建获取炸蓝是否插入任务
		task_create_io_get();
    }

    func_disable_slider();
}


// 异常/拔锅弹框显示：暂停运行、关风扇/加热并提示插入炸蓝
static void func_pop_up()
{
	screensaver_time = 600*30;  //30分钟进入屏保时钟界面
	//请插入炸蓝立即关闭风扇
	timer_fan_off_cb();
	//停止运行关闭发热丝
	yk_v_stop_run_state = true;
	yk_f_run_start_pause_clicked(0);
	//请插入炸蓝立即关闭风扇弹框显示
	lv_obj_clear_flag(scr->container_6_pop_up, LV_OBJ_FLAG_HIDDEN);
	#if CUSTOM_SCREEN_3_BAKE_LOG
		//获取到的实际温度°C
		int16_t ntc_voltage_i = 0;
		for (ntc_voltage_i = 0; ntc_voltage_i < 251; ++ntc_voltage_i) {
			if (ntc_voltage_value[ntc_voltage_i][1] < ntc_voltage) {
				//yk_printf("get ntc_voltage_i == %d \n",ntc_voltage_i-1);
				break;
			}
		}
		uint8_t  label0_io_get_fryer_i[100];
		uint8_t  time_difference[100][100];
		uint8_t  combined_label[1024*10] = {0};

		io_count_var[io_count_v][4] = yk_v_hor_value;
		io_count_var[io_count_v][5] = yk_v_min_value;
		io_count_var[io_count_v][6] = yk_v_sec_value;
		// 在需要更新最大值的地方添加如下代码
		io_get_fryer_i > io_get_fryer_i_max?io_get_fryer_i_max = io_get_fryer_i : io_get_fryer_i;
		sprintf(label0_io_get_fryer_i,
			"%d {%d°C}(%d) [%d°C] %d max%d\n"
			"[%d]\n"
			"yk_v_food_t_f + ntc_t_difference == [%d°C] %s\n", // 动态生成条件字符串
			ntc_voltage,ntc_voltage_i,heating_off,yk_v_food_t_f,io_get_fryer_i,io_get_fryer_i_max,
			ntc_t_difference,
			yk_v_food_t_f + ntc_t_difference,(ntc_voltage_i >= yk_v_food_t_f + ntc_t_difference + 1) ? "off" : "on"
		);
				
		for (uint8_t i = 0; i <= io_count_v; ++i) {
			Time t1 = {io_count_var[i][4], io_count_var[i][5], io_count_var[i][6]}; 
			Time t2 = {io_count_var[i][1], io_count_var[i][2], io_count_var[i][3]};
			Time result = time_subtract(t1,t2);
			sprintf(time_difference[i],
				"io_count_var[%d][0] = %d \n%02d:%02d:%02d-%02d:%02d:%02d=%02d:%02d:%02d\n",
				i, io_count_var[i][0],
				io_count_var[i][1], io_count_var[i][2], io_count_var[i][3],
				io_count_var[i][4], io_count_var[i][5], io_count_var[i][6],
				result.hours, result.minutes, result.seconds
			);
		}
		
		// 将第一个字符串添加到 combined_label
		strcat(combined_label, label0_io_get_fryer_i);
		// 将后面日志打印到 combined_label 中
		for (uint8_t i = 1; i <= io_count_v; ++i) {
			strcat(combined_label, time_difference[i]);
		}

		lv_label_set_text(scr->label_9_info, combined_label);
		lv_obj_set_style_text_color(scr->label_9_info, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
	#endif
}

// 删除获取炸蓝插入任务并复位计数
static void task_del_io_get()
{
	if (task_io_get) {
		lv_timer_del(task_io_get);
		task_io_get = NULL;
		io_get_fryer_i = 0;
	}
}

// 炸蓝状态防抖：连续采样 20 次都为“未插入”则返回 true
static bool check_yk_io_get_fryer(void) {
    for (int i = 0; i < 20; i++) {
        if (yk_io_get_fryer() == 0) {
            return false;
        }
		#if YK_USE_UI_BUILDER
        rt_thread_mdelay(1);
		//rt_hw_us_delay(1);
		#endif
    }
    return true;
}

// 判断 NTC 是否异常：异常返回 1，正常返回 0
static uint8_t func_ntc_error()  //判断NTC问题是否异常函数
{
	//判断NTC是否异常
	if (ntc_voltage >= 25000)
	{
		//NTC开路
		lv_label_set_text_fmt(scr->label_6_pop_up_1_insert_basket,"%s" "%d","NTC ERROR:",ntc_voltage);
		if (yk_v_stop_run_state == false)return 1;
		func_pop_up();
		return 1;
	}
	else  if(ntc_voltage > 0 && ntc_voltage < ntc_voltage_value[255+1][1]){
		//超温异常提示
		
		lv_label_set_text_fmt(scr->label_6_pop_up_1_insert_basket,"%s" "%d","TEMPERATURE ERROR:",ntc_voltage);
		if (yk_v_stop_run_state == false)return 1;
		screensaver_time = 600*30;  //30分钟进入屏保时钟界面
		func_pop_up();
		return 1;
	}
	else  if(ntc_voltage <= 0){
		//NTC短路
		lv_label_set_text_fmt(scr->label_6_pop_up_1_insert_basket,"%s" "%d","NTC ERROR:",ntc_voltage);
		if (yk_v_stop_run_state == false)return 1;
		func_pop_up();
		return 1;
	}
	else  {
		//正常
		return 0;
	}
}

// 获取炸蓝是否插入任务：运行/暂停两种状态下分别处理弹框与恢复
static void task_io_get_cb()  //获取炸蓝是否插入任务
{
	if(func_ntc_error())return;  //判断NTC问题是否异常,异常返回1退出下面代码不执行，正常返回0,下面代码执行
	if (yk_v_stop_run_state)
	{
		//获取炸蓝是否插入
		if (yk_io_get_fryer()==0)
		{
			//炸蓝已经插入
			io_get_fryer_i = 0;
		}
		else {
			if (check_yk_io_get_fryer()) {
				if(io_get_fryer_i < 15) {
					io_get_fryer_i++;
					if (io_get_fryer_i > 10) {
						//请插入炸蓝立即关闭风扇,并立即显示弹框
						func_pop_up();
					}
				}
			}
		}
	}
	else {
		//获取炸蓝是否插入
		if (yk_io_get_fryer()==0)
		{
			//炸蓝已经插入
			io_get_fryer_i = 0;
			yk_printf("222222222 == %d\n",yk_io_get_fryer());
			lv_obj_add_flag(scr->container_6_pop_up, LV_OBJ_FLAG_HIDDEN);

			yk_v_stop_run_state = false;
			yk_f_run_start_pause_clicked(0);
		}
		else {
			if (check_yk_io_get_fryer()) {
				if (io_get_fryer_i < 15) {
					io_get_fryer_i++;
					if (io_get_fryer_i > 10) {
						//请插入炸蓝立即关闭风扇
						#if YK_USE_UI_BUILDER
						timer_fan_off_cb();
						#endif
						printf("Please insert the blue fry** %d\n",io_get_fryer_i);
					}
				}
			}
		}
	}
}

// 创建获取炸蓝是否插入任务（周期轮询）
static void task_create_io_get(void)
{
	if (task_io_get) {
		lv_timer_del(task_io_get);
		task_io_get = NULL;
	}
	io_get_fryer_i = 0;
	if(!task_io_get)task_io_get = lv_timer_create(task_io_get_cb,10,NULL);
	lv_timer_ready(task_io_get);
}


static void task_arc_shake_cb()  //炸蓝运行到一半执行抖动任务
{
	arc_countdown_half_i++;
	if (arc_countdown_half_j < 3)
	{
        if (arc_countdown_half_i == 40) {
            arc_countdown_half_j++;
            arc_countdown_half_i = 0;
            #if YK_USE_UI_BUILDER
            yk_io_set_beep(0);
            #endif
            lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
        }
		//
		if (arc_countdown_half_i == 1) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(1);
			#endif
            lv_obj_clear_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+1*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(0);
			#endif
            lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+2*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(1);
			#endif
            lv_obj_clear_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+3*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(0);
			#endif
            lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+4*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(1);
			#endif
            lv_obj_clear_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+5*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(0);
			#endif
            lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+6*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(1);
			#endif
            lv_obj_clear_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
		else if (arc_countdown_half_i == 1+7*4) {
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(0);
			#endif
            lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
		}
	}
	else {
		//lv_img_set_src(scr->image_2_shake, LVGL_IMAGE_PATH(BG_shake1.png));
		if (task_arc_shake) {
			arc_countdown_half_i = 0;
			arc_countdown_half_j = 0;
			//lv_image_set_rotation(scr->image_2_shake,0);
			#if YK_USE_UI_BUILDER
			yk_io_set_beep(0);
			#endif
            lv_obj_add_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
			lv_timer_del(task_arc_shake);
			task_arc_shake = NULL;
		}
	}

	yk_printf("arc_countdown_half_i == %u \n",arc_countdown_half_i);
}


//删除arc倒计时任务、删除获取炸蓝插入任务、删除获取电压值任务
void task_del(void)
{
	//删除arc倒计时任务
	if (task_arc) {
		lv_timer_del(task_arc);
		task_arc = NULL;
		io_get_fryer_i = 0;
	}
	//删除获取炸蓝插入任务
	task_del_io_get();
	#if YK_USE_UI_BUILDER
	//删除获取电压值任务
	yk_f_psadc_del();
	#endif
}

// 倒计时/控温回调：更新 arc 与时间，按温度阈值开关发热丝
static void task_arc_cb(lv_timer_t *task_t)
{
	if (arc_countdown >= 0) {
		time_countdown++;
		if (time_countdown == 5) {
			time_countdown = 0;
            arc_s--;
			if (arc_s < 0) {
                arc_m--;
				yk_printf("arc_m == %d \n",arc_m);
                arc_s = 59;
			}
			lv_label_set_text_fmt(scr->label_run_timer, "%02d:%02d",arc_m,arc_s);
		}

		if (arc_countdown) {
			arc_countdown --;
		}

		lv_arc_set_value(scr->arc_1,arc_countdown);

		//时间运行到一半执行炸蓝抖动任务
        #if 1
		if (arc_countdown_half == arc_countdown) {
			//lv_img_set_src(scr->image_2_shake, LVGL_IMAGE_PATH(BG_shake2.png));
            lv_obj_clear_flag(scr->label_run_shake, LV_OBJ_FLAG_HIDDEN);
			if (task_arc_shake == NULL)task_arc_shake = lv_timer_create(task_arc_shake_cb,50,NULL);
		}
        #endif

		//获取到的实际温度°C
		int16_t ntc_voltage_i = 0;
		for (ntc_voltage_i = 0; ntc_voltage_i < 251; ++ntc_voltage_i) {
			if (ntc_voltage_value[ntc_voltage_i][1] < ntc_voltage) {
				//yk_printf("get ntc_voltage_i == %d \n",ntc_voltage_i-1);
				break;
			}
		}

		//如果设置的时间大于8分钟就提前60秒关发热丝
		if (arc_countdown_half*2/5/60 >= 8 && arc_countdown/5 <= 30)
		{
			//发热丝关
			yk_io_set_heating_off();
		}
		else
		{
			#if 1
			// 温控逻辑：按 NTC 电压映射温度阈值控制发热丝开关
			if (ntc_voltage <= ntc_voltage_value[yk_v_food_t_f + ntc_t_difference][1])
			{
				//发热丝关
				yk_io_set_heating_off();
			}
			else if (ntc_voltage > ntc_voltage_value[yk_v_food_t_f + ntc_t_difference - 8][1])
			{
				//发热丝开
				yk_io_set_heating_on();
			}
			#endif
		}
		
		if (arc_countdown == 0) {
			yk_printf("倒计时完成：%d:%d\n",arc_m,arc_s);
			#if YK_USE_UI_BUILDER
			//发热丝关
			yk_io_set_heating(0);
			#endif
			yk_printf("ntc_voltage off == %d\n",ntc_voltage);
			//回到主界面
			yk_f_cancel();
		}
		//yk_printf("arc_countdown == %d  arc_countdown_half == %d\n",arc_countdown,arc_countdown_half);
        #if CUSTOM_SCREEN_3_BAKE_LOG
			uint8_t  label0_io_get_fryer_i[100];
			uint8_t  time_difference[100][100];
			uint8_t  combined_label[1024*10] = {0};

			io_count_var[io_count_v][4] = yk_v_hor_value;
			io_count_var[io_count_v][5] = yk_v_min_value;
			io_count_var[io_count_v][6] = yk_v_sec_value;
			// 在需要更新最大值的地方添加如下代码
			io_get_fryer_i > io_get_fryer_i_max?io_get_fryer_i_max = io_get_fryer_i : io_get_fryer_i;
			sprintf(label0_io_get_fryer_i,
				"%d {%d°C}(%d) [%d°C] %d max%d\n"
				"[%d]\n"
				"yk_v_food_t_f + ntc_t_difference == [%d°C] %s\n", // 动态生成条件字符串
				ntc_voltage,ntc_voltage_i,heating_off,yk_v_food_t_f,io_get_fryer_i,io_get_fryer_i_max,
				ntc_t_difference,
				yk_v_food_t_f + ntc_t_difference,(ntc_voltage_i >= yk_v_food_t_f + ntc_t_difference + 1) ? "off" : "on"
			);
					
			for (uint8_t i = 0; i <= io_count_v; ++i) {
				Time t1 = {io_count_var[i][4], io_count_var[i][5], io_count_var[i][6]}; 
				Time t2 = {io_count_var[i][1], io_count_var[i][2], io_count_var[i][3]};
				Time result = time_subtract(t1,t2);
				sprintf(time_difference[i],
                    "io_count_var[%d][0] = %d \n%02d:%02d:%02d-%02d:%02d:%02d=%02d:%02d:%02d\n",
                    i, io_count_var[i][0],
                    io_count_var[i][1], io_count_var[i][2], io_count_var[i][3],
                    io_count_var[i][4], io_count_var[i][5], io_count_var[i][6],
                    result.hours, result.minutes, result.seconds
                );
			}
			
			// 将第一个字符串添加到 combined_label
			strcat(combined_label, label0_io_get_fryer_i);
			// 将后面日志打印到 combined_label 中
			for (uint8_t i = 1; i <= io_count_v; ++i) {
				strcat(combined_label, time_difference[i]);
			}

			lv_label_set_text(scr->label_9_info, combined_label);
			lv_obj_set_style_text_color(scr->label_9_info, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
		#endif
	}
}

// 创建倒计时与控温任务：初始化 arc/时间并启动加热、风扇与检测
static void task_create(uint16_t food_min_value)
{
    food_min = food_min_value;
	arc_m = food_min_value;
	arc_s = 0;

	//计时倒计时时间：秒为单位
	time_countdown = arc_m*60+arc_s;
	yk_printf("arc_m == %d\n",arc_m);
	yk_printf("arc_s == %d\n",arc_s);
	yk_printf("time_countdown == %d\n",time_countdown);

	//设置arc进度范围的最大值
	arc_countdown = 5*time_countdown;
	arc_countdown_half = arc_countdown/2;
	time_countdown = 0;  //把这个等于0,用户时间和圆弧在一个任务里做判断用，圆弧走100次，时间走一次
	//设置arc值范围
	lv_arc_set_range(scr->arc_1,0,arc_countdown);
	//设置arc值
	lv_arc_set_value(scr->arc_1,arc_countdown);
	//
	io_get_fryer_i = 0;

	//创建arc任务
	if(task_arc==NULL)
	{
		task_arc = lv_timer_create(task_arc_cb,200,NULL);
		lv_timer_ready(task_arc);
	}

    #if YK_USE_UI_BUILDER
    //创建获取电压值任务
    yk_f_psadc_create();
    #endif
	//判断不同温度调差值
    func_t_mft();
	//创建获取炸蓝是否插入任务
	task_create_io_get();
	//发热丝开
	yk_io_set_heating_on();
	//风扇开
	task_create_fan_on();
}


// 暂停运行（占位函数）
void yk_f_pause(void)
{ 
    yk_printf("yk_f_pause");
}

// 运行：进入运行界面并启动/恢复倒计时
void yk_f_run(void)
{ 
    yk_printf("yk_f_run\n");
    if (task_arc == NULL)
    {
        //修改运行图片
        lv_img_set_src(scr->image_run, food_image_paths_run[yk_food_flag]);
        //显示运行容器
        lv_obj_clear_flag(scr->container_2_3_run, LV_OBJ_FLAG_HIDDEN);
        //进入立即刷新倒计时时间
	    lv_label_set_text_fmt(scr->label_run_timer, "%02d:%02d",yk_v_food_min,0);

        // //隐藏控件
        // lv_obj_add_flag(scr->label_1_airfry, LV_OBJ_FLAG_HIDDEN);
        // lv_obj_add_flag(scr->label_2_bake, LV_OBJ_FLAG_HIDDEN);
        // lv_obj_add_flag(scr->label_3_preheat, LV_OBJ_FLAG_HIDDEN);
        // lv_obj_add_flag(scr->label_4_dehydrate, LV_OBJ_FLAG_HIDDEN);

		//测试发热丝开合次数
		#if CUSTOM_SCREEN_3_BAKE_LOG
		io_count_v++;
		io_count_var[io_count_v][1] = yk_v_hor_value;
		io_count_var[io_count_v][2] = yk_v_min_value;
		io_count_var[io_count_v][3] = yk_v_sec_value;
		#endif
    }

    yk_f_run_start_pause_clicked(true);
}


// 点击取消运行：停止并回到主界面状态
void screen_02_main_label_2_custom_clicked(void)
{
    yk_f_cancel();
}


#if CUSTOM_SCREEN_3_BAKE_LOG
// 温度差值调试：增加补偿值
void ntc_t_difference_add_custom_clicked()
{
	ntc_t_difference++;
	printf("ntc_t_difference:%d\n", ntc_t_difference);
}
// 温度差值调试：减少补偿值（长按）
void ntc_t_difference_sub_custom_long_pressed()
{
	ntc_t_difference -=2;
	printf("ntc_t_difference:%d\n", ntc_t_difference);
}
#endif


// 运行界面首次创建：缓存当前 screen 指针
void screen_02_main_run_custom_created(void)
{
    // 获取当前屏幕
    scr = screen_02_main_get(&ui_manager);

	#if CUSTOM_SCREEN_3_BAKE_LOG
	//给label_9_info添加可以点击的属性
	lv_obj_add_flag(scr->label_9_info, LV_OBJ_FLAG_CLICKABLE);
	//给label_9_info添加点击事件
	lv_obj_add_event_cb(scr->label_9_info, ntc_t_difference_add_custom_clicked, LV_EVENT_CLICKED, NULL);
	//给label_9_info添加长按事件
	lv_obj_add_event_cb(scr->label_9_info, ntc_t_difference_sub_custom_long_pressed, LV_EVENT_LONG_PRESSED, NULL);
	#endif
}
