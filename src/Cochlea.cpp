#include "plugin.hpp"
#include "NeuronConstants.hpp"
#include <algorithm>
#include <thread>
#include <mutex>

std::mutex m;

rfft = rack::dsp::RealFFT(1024);

const float hamming[1024] = {0.08, 0.08000868, 0.0800347, 0.08007808, 0.08013881, 0.08021689, 0.08031231, 0.08042507, 0.08055517, 0.0807026,
       						0.08086736, 0.08104944, 0.08124883, 0.08146552, 0.08169951, 0.08195079, 0.08221935, 0.08250518, 0.08280827, 0.0831286,
       						0.08346617, 0.08382096, 0.08419296, 0.08458215, 0.08498853, 0.08541206, 0.08585275, 0.08631057, 0.0867855 , 0.08727753,
							0.08778663, 0.0883128 , 0.088856  , 0.08941623, 0.08999345, 0.09058764, 0.09119879, 0.09182687, 0.09247186, 0.09313373,
							0.09381245, 0.09450801, 0.09522037, 0.09594951, 0.0966954 , 0.09745802, 0.09823733, 0.0990333 , 0.09984591, 0.10067512,
							0.1015209 , 0.10238323, 0.10326206, 0.10415737, 0.10506912, 0.10599728, 0.10694181, 0.10790267, 0.10887984, 0.10987326,
							0.11088292, 0.11190876, 0.11295075, 0.11400885, 0.11508302, 0.11617322, 0.11727941, 0.11840154, 0.11953958, 0.12069348,
							0.12186319, 0.12304868, 0.1242499 , 0.1254668 , 0.12669934, 0.12794747, 0.12921114, 0.13049031, 0.13178493, 0.13309495,
							0.13442031, 0.13576098, 0.1371169 , 0.13848801, 0.13987427, 0.14127562, 0.14269202, 0.1441234 , 0.14556972, 0.14703091,
							0.14850693, 0.14999772, 0.15150322, 0.15302337, 0.15455813, 0.15610742, 0.1576712 , 0.15924939, 0.16084195, 0.16244882,
							0.16406992, 0.16570521, 0.16735462, 0.16901808, 0.17069554, 0.17238693, 0.17409219, 0.17581125, 0.17754405, 0.17929052,
							0.1810506 , 0.18282422, 0.18461131, 0.18641181, 0.18822565, 0.19005275, 0.19189306, 0.1937465 , 0.19561301, 0.1974925 ,
							0.19938492, 0.20129018, 0.20320822, 0.20513897, 0.20708234, 0.20903828, 0.2110067 , 0.21298753, 0.2149807 , 0.21698613,
							0.21900374, 0.22103346, 0.22307522, 0.22512893, 0.22719452, 0.2292719 , 0.23136101, 0.23346177, 0.23557408, 0.23769788,
							0.23983308, 0.24197961, 0.24413738, 0.24630631, 0.24848632, 0.25067733, 0.25287925, 0.255092  , 0.25731549, 0.25954966,
							0.2617944 , 0.26404964, 0.26631528, 0.26859125, 0.27087746, 0.27317382, 0.27548025, 0.27779665, 0.28012295, 0.28245905,
							0.28480486, 0.2871603 , 0.28952528, 0.29189971, 0.2942835 , 0.29667656, 0.29907879, 0.30149011, 0.30391043, 0.30633966,
							0.3087777 , 0.31122447, 0.31367986, 0.31614379, 0.31861617, 0.32109689, 0.32358588, 0.32608303, 0.32858825, 0.33110144,
							0.33362251, 0.33615137, 0.33868792, 0.34123206, 0.3437837 , 0.34634275, 0.34890909, 0.35148265, 0.35406332, 0.356651  ,
							0.3592456 , 0.36184702, 0.36445515, 0.36706991, 0.3696912 , 0.37231891, 0.37495294, 0.3775932 , 0.38023958, 0.382892  ,
							0.38555034, 0.3882145 , 0.39088439, 0.39355991, 0.39624095, 0.39892741, 0.4016192 , 0.4043162 , 0.40701833, 0.40972547,
							0.41243752, 0.41515439, 0.41787596, 0.42060215, 0.42333283, 0.42606792, 0.42880731, 0.43155089, 0.43429856, 0.43705022,
							0.43980576, 0.44256509, 0.44532808, 0.44809465, 0.45086469, 0.45363809, 0.45641474, 0.45919455, 0.46197741, 0.46476321,
							0.46755185, 0.47034322, 0.47313722, 0.47593374, 0.47873268, 0.48153393, 0.48433739, 0.48714294, 0.48995049, 0.49275993,
							0.49557115, 0.49838404, 0.50119851, 0.50401444, 0.50683172, 0.50965026, 0.51246994, 0.51529067, 0.51811232, 0.5209348 ,
							0.523758  , 0.52658181, 0.52940612, 0.53223084, 0.53505585, 0.53788104, 0.54070632, 0.54353157, 0.54635669, 0.54918156,
							0.55200609, 0.55483017, 0.55765369, 0.56047654, 0.56329862, 0.56611982, 0.56894003, 0.57175916, 0.57457708, 0.5773937 ,
							0.58020891, 0.58302261, 0.58583468, 0.58864502, 0.59145352, 0.59426009, 0.59706461, 0.59986697, 0.60266708, 0.60546483,
							0.6082601 , 0.6110528 , 0.61384282, 0.61663005, 0.61941439, 0.62219574, 0.62497399, 0.62774903, 0.63052076, 0.63328907,
							0.63605387, 0.63881504, 0.64157249, 0.6443261 , 0.64707578, 0.64982142, 0.65256291, 0.65530016, 0.65803307, 0.66076151,
							0.66348541, 0.66620464, 0.66891911, 0.67162872, 0.67433337, 0.67703295, 0.67972735, 0.68241649, 0.68510026, 0.68777855,
							0.69045126, 0.6931183 , 0.69577957, 0.69843496, 0.70108437, 0.7037277 , 0.70636486, 0.70899575, 0.71162025, 0.71423829,
							0.71684975, 0.71945454, 0.72205256, 0.72464371, 0.7272279 , 0.72980502, 0.73237499, 0.73493769, 0.73749305, 0.74004095,
							0.74258131, 0.74511403, 0.747639  , 0.75015615, 0.75266537, 0.75516656, 0.75765964, 0.76014451, 0.76262107, 0.76508924,
							0.76754891, 0.77      , 0.77244241, 0.77487606, 0.77730084, 0.77971668, 0.78212347, 0.78452113, 0.78690956, 0.78928868,
							0.79165839, 0.79401861, 0.79636925, 0.79871022, 0.80104143, 0.80336279, 0.80567422, 0.80797562, 0.81026692, 0.81254802,
							0.81481884, 0.81707929, 0.81932929, 0.82156875, 0.82379759, 0.82601573, 0.82822307, 0.83041955, 0.83260506, 0.83477954,
							0.8369429 , 0.83909506, 0.84123594, 0.84336545, 0.84548352, 0.84759006, 0.849685  , 0.85176826, 0.85383976, 0.85589942,
							0.85794716, 0.85998291, 0.86200658, 0.86401812, 0.86601742, 0.86800443, 0.86997907, 0.87194125, 0.87389092, 0.87582799,
							0.87775239, 0.87966405, 0.8815629 , 0.88344887, 0.88532187, 0.88718185, 0.88902874, 0.89086245, 0.89268294, 0.89449011,
							0.89628392, 0.89806428, 0.89983114, 0.90158442, 0.90332407, 0.90505   , 0.90676217, 0.9084605 , 0.91014494, 0.9118154 ,
							0.91347185, 0.9151142 , 0.91674241, 0.9183564 , 0.91995612, 0.9215415 , 0.9231125 , 0.92466904, 0.92621107, 0.92773853,
							0.92925137, 0.93074952, 0.93223293, 0.93370154, 0.9351553 , 0.93659416, 0.93801806, 0.93942694, 0.94082075, 0.94219944,
							0.94356296, 0.94491126, 0.94624428, 0.94756198, 0.94886431, 0.95015121, 0.95142263, 0.95267854, 0.95391888, 0.95514361,
							0.95635267, 0.95754603, 0.95872364, 0.95988545, 0.96103143, 0.96216152, 0.96327568, 0.96437388, 0.96545607, 0.96652221,
							0.96757226, 0.96860618, 0.96962393, 0.97062548, 0.97161078, 0.9725798 , 0.9735325 , 0.97446885, 0.97538881, 0.97629234,
							0.97717942, 0.97805   , 0.97890406, 0.97974156, 0.98056247, 0.98136677, 0.98215441, 0.98292538, 0.98367963, 0.98441715,
							0.98513791, 0.98584187, 0.98652902, 0.98719932, 0.98785275, 0.98848928, 0.9891089 , 0.98971157, 0.99029729, 0.99086601,
							0.99141773, 0.99195241, 0.99247005, 0.99297062, 0.99345411, 0.99392048, 0.99436974, 0.99480185, 0.99521681, 0.99561459,
							0.99599519, 0.99635858, 0.99670477, 0.99703372, 0.99734543, 0.99763989, 0.99791708, 0.99817701, 0.99841964, 0.99864499,
							0.99885303, 0.99904377, 0.99921718, 0.99937328, 0.99951204, 0.99963347, 0.99973757, 0.99982432, 0.99989372, 0.99994577,
							0.99998048, 0.99999783, 0.99999783, 0.99998048, 0.99994577, 0.99989372, 0.99982432, 0.99973757, 0.99963347, 0.99951204,
							0.99937328, 0.99921718, 0.99904377, 0.99885303, 0.99864499, 0.99841964, 0.99817701, 0.99791708, 0.99763989, 0.99734543,
							0.99703372, 0.99670477, 0.99635858, 0.99599519, 0.99561459, 0.99521681, 0.99480185, 0.99436974, 0.99392048, 0.99345411,
							0.99297062, 0.99247005, 0.99195241, 0.99141773, 0.99086601, 0.99029729, 0.98971157, 0.9891089 , 0.98848928, 0.98785275,
							0.98719932, 0.98652902, 0.98584187, 0.98513791, 0.98441715, 0.98367963, 0.98292538, 0.98215441, 0.98136677, 0.98056247,
							0.97974156, 0.97890406, 0.97805   , 0.97717942, 0.97629234, 0.97538881, 0.97446885, 0.9735325 , 0.9725798 , 0.97161078,
							0.97062548, 0.96962393, 0.96860618, 0.96757226, 0.96652221, 0.96545607, 0.96437388, 0.96327568, 0.96216152, 0.96103143,
							0.95988545, 0.95872364, 0.95754603, 0.95635267, 0.95514361, 0.95391888, 0.95267854, 0.95142263, 0.95015121, 0.94886431,
							0.94756198, 0.94624428, 0.94491126, 0.94356296, 0.94219944, 0.94082075, 0.93942694, 0.93801806, 0.93659416, 0.9351553 ,
							0.93370154, 0.93223293, 0.93074952, 0.92925137, 0.92773853, 0.92621107, 0.92466904, 0.9231125 , 0.9215415 , 0.91995612,
							0.9183564 , 0.91674241, 0.9151142 , 0.91347185, 0.9118154 , 0.91014494, 0.9084605 , 0.90676217, 0.90505   , 0.90332407,
							0.90158442, 0.89983114, 0.89806428, 0.89628392, 0.89449011, 0.89268294, 0.89086245, 0.88902874, 0.88718185, 0.88532187,
							0.88344887, 0.8815629 , 0.87966405, 0.87775239, 0.87582799, 0.87389092, 0.87194125, 0.86997907, 0.86800443, 0.86601742,
							0.86401812, 0.86200658, 0.85998291, 0.85794716, 0.85589942, 0.85383976, 0.85176826, 0.849685  , 0.84759006, 0.84548352,
							0.84336545, 0.84123594, 0.83909506, 0.8369429 , 0.83477954, 0.83260506, 0.83041955, 0.82822307, 0.82601573, 0.82379759,
							0.82156875, 0.81932929, 0.81707929, 0.81481884, 0.81254802, 0.81026692, 0.80797562, 0.80567422, 0.80336279, 0.80104143,
							0.79871022, 0.79636925, 0.79401861, 0.79165839, 0.78928868, 0.78690956, 0.78452113, 0.78212347, 0.77971668, 0.77730084,
							0.77487606, 0.77244241, 0.77      , 0.76754891, 0.76508924, 0.76262107, 0.76014451, 0.75765964, 0.75516656, 0.75266537,
							0.75015615, 0.747639  , 0.74511403, 0.74258131, 0.74004095, 0.73749305, 0.73493769, 0.73237499, 0.72980502, 0.7272279 ,
							0.72464371, 0.72205256, 0.71945454, 0.71684975, 0.71423829, 0.71162025, 0.70899575, 0.70636486, 0.7037277 , 0.70108437,
							0.69843496, 0.69577957, 0.6931183 , 0.69045126, 0.68777855, 0.68510026, 0.68241649, 0.67972735, 0.67703295, 0.67433337,
							0.67162872, 0.66891911, 0.66620464, 0.66348541, 0.66076151, 0.65803307, 0.65530016, 0.65256291, 0.64982142, 0.64707578,
							0.6443261 , 0.64157249, 0.63881504, 0.63605387, 0.63328907, 0.63052076, 0.62774903, 0.62497399, 0.62219574, 0.61941439,
							0.61663005, 0.61384282, 0.6110528 , 0.6082601 , 0.60546483, 0.60266708, 0.59986697, 0.59706461, 0.59426009, 0.59145352,
							0.58864502, 0.58583468, 0.58302261, 0.58020891, 0.5773937 , 0.57457708, 0.57175916, 0.56894003, 0.56611982, 0.56329862,
							0.56047654, 0.55765369, 0.55483017, 0.55200609, 0.54918156, 0.54635669, 0.54353157, 0.54070632, 0.53788104, 0.53505585,
							0.53223084, 0.52940612, 0.52658181, 0.523758  , 0.5209348 , 0.51811232, 0.51529067, 0.51246994, 0.50965026, 0.50683172,
							0.50401444, 0.50119851, 0.49838404, 0.49557115, 0.49275993, 0.48995049, 0.48714294, 0.48433739, 0.48153393, 0.47873268,
							0.47593374, 0.47313722, 0.47034322, 0.46755185, 0.46476321, 0.46197741, 0.45919455, 0.45641474, 0.45363809, 0.45086469,
							0.44809465, 0.44532808, 0.44256509, 0.43980576, 0.43705022, 0.43429856, 0.43155089, 0.42880731, 0.42606792, 0.42333283,
							0.42060215, 0.41787596, 0.41515439, 0.41243752, 0.40972547, 0.40701833, 0.4043162 , 0.4016192 , 0.39892741, 0.39624095,
							0.39355991, 0.39088439, 0.3882145 , 0.38555034, 0.382892  , 0.38023958, 0.3775932 , 0.37495294, 0.37231891, 0.3696912 ,
							0.36706991, 0.36445515, 0.36184702, 0.3592456 , 0.356651  , 0.35406332, 0.35148265, 0.34890909, 0.34634275, 0.3437837 ,
							0.34123206, 0.33868792, 0.33615137, 0.33362251, 0.33110144, 0.32858825, 0.32608303, 0.32358588, 0.32109689, 0.31861617,
							0.31614379, 0.31367986, 0.31122447, 0.3087777 , 0.30633966, 0.30391043, 0.30149011, 0.29907879, 0.29667656, 0.2942835 ,
							0.29189971, 0.28952528, 0.2871603 , 0.28480486, 0.28245905, 0.28012295, 0.27779665, 0.27548025, 0.27317382, 0.27087746,
							0.26859125, 0.26631528, 0.26404964, 0.2617944 , 0.25954966, 0.25731549, 0.255092  , 0.25287925, 0.25067733, 0.24848632,
							0.24630631, 0.24413738, 0.24197961, 0.23983308, 0.23769788, 0.23557408, 0.23346177, 0.23136101, 0.2292719 , 0.22719452,
							0.22512893, 0.22307522, 0.22103346, 0.21900374, 0.21698613, 0.2149807 , 0.21298753, 0.2110067 , 0.20903828, 0.20708234,
							0.20513897, 0.20320822, 0.20129018, 0.19938492, 0.1974925 , 0.19561301, 0.1937465 , 0.19189306, 0.19005275, 0.18822565,
							0.18641181, 0.18461131, 0.18282422, 0.1810506 , 0.17929052, 0.17754405, 0.17581125, 0.17409219, 0.17238693, 0.17069554,
							0.16901808, 0.16735462, 0.16570521, 0.16406992, 0.16244882, 0.16084195, 0.15924939, 0.1576712 , 0.15610742, 0.15455813,
							0.15302337, 0.15150322, 0.14999772, 0.14850693, 0.14703091, 0.14556972, 0.1441234 , 0.14269202, 0.14127562, 0.13987427,
							0.13848801, 0.1371169 , 0.13576098, 0.13442031, 0.13309495, 0.13178493, 0.13049031, 0.12921114, 0.12794747, 0.12669934,
							0.1254668 , 0.1242499 , 0.12304868, 0.12186319, 0.12069348, 0.11953958, 0.11840154, 0.11727941, 0.11617322, 0.11508302,
							0.11400885, 0.11295075, 0.11190876, 0.11088292, 0.10987326, 0.10887984, 0.10790267, 0.10694181, 0.10599728, 0.10506912,
							0.10415737, 0.10326206, 0.10238323, 0.1015209 , 0.10067512, 0.09984591, 0.0990333 , 0.09823733, 0.09745802, 0.0966954 ,
							0.09594951, 0.09522037, 0.09450801, 0.09381245, 0.09313373, 0.09247186, 0.09182687, 0.09119879, 0.09058764, 0.08999345,
							0.08941623, 0.088856  , 0.0883128 , 0.08778663, 0.08727753, 0.0867855 , 0.08631057, 0.08585275, 0.08541206, 0.08498853,
							0.08458215, 0.08419296, 0.08382096, 0.08346617, 0.0831286 , 0.08280827, 0.08250518, 0.08221935, 0.08195079, 0.08169951,
							0.08146552, 0.08124883, 0.08104944, 0.08086736, 0.0807026 , 0.08055517, 0.08042507, 0.08031231, 0.08021689, 0.08013881,
							0.08007808, 0.0800347 , 0.08000868, 0.08};

struct Cochlea : Module {
	enum ParamId {
		REF_PARAM,
		RESP_PARAM,
		SENS_PARAM,
		PARAMS_LEN
	};
	enum InputId {
		AUDIO_INPUT,
		INPUTS_LEN
	};
	enum OutputId {
		A5_OUTPUT,
		BB5_OUTPUT,
		B5_OUTPUT,
		C5_OUTPUT,
		DB5_OUTPUT,
		D5_OUTPUT,
		EB5_OUTPUT,
		E5_OUTPUT,
		F5_OUTPUT,
		GB5_OUTPUT,
		G5_OUTPUT,
		AB5_OUTPUT,
		A4_OUTPUT,
		BB4_OUTPUT,
		B4_OUTPUT,
		C4_OUTPUT,
		DB4_OUTPUT,
		D4_OUTPUT,
		EB4_OUTPUT,
		E4_OUTPUT,
		F4_OUTPUT,
		GB4_OUTPUT,
		G4_OUTPUT,
		AB4_OUTPUT,
		A3_OUTPUT,
		BB3_OUTPUT,
		B3_OUTPUT,
		C3_OUTPUT,
		DB3_OUTPUT,
		D3_OUTPUT,
		EB3_OUTPUT,
		E3_OUTPUT,
		F3_OUTPUT,
		GB3_OUTPUT,
		G3_OUTPUT,
		AB3_OUTPUT,
		A2_OUTPUT,
		BB2_OUTPUT,
		B2_OUTPUT,
		C2_OUTPUT,
		DB2_OUTPUT,
		D2_OUTPUT,
		EB2_OUTPUT,
		E2_OUTPUT,
		F2_OUTPUT,
		GB2_OUTPUT,
		G2_OUTPUT,
		AB2_OUTPUT,
		A1_OUTPUT,
		BB1_OUTPUT,
		B1_OUTPUT,
		C1_OUTPUT,
		DB1_OUTPUT,
		D1_OUTPUT,
		EB1_OUTPUT,
		E1_OUTPUT,
		F1_OUTPUT,
		GB1_OUTPUT,
		G1_OUTPUT,
		AB1_OUTPUT,
		A0_OUTPUT,
		BB0_OUTPUT,
		B0_OUTPUT,
		C0_OUTPUT,
		DB0_OUTPUT,
		D0_OUTPUT,
		EB0_OUTPUT,
		E0_OUTPUT,
		F0_OUTPUT,
		GB0_OUTPUT,
		G0_OUTPUT,
		AB0_OUTPUT,
		OUTPUTS_LEN
	};
	enum LightId {
		LIGHTS_LEN
	};

	int buff_counter = 0;
	static const int buff_len = 1024;
	float buff[buff_len];
	float mems[OUTPUTS_LEN];

	Cochlea() {
		config(PARAMS_LEN, INPUTS_LEN, OUTPUTS_LEN, LIGHTS_LEN);
		configParam(REF_PARAM, 0.f, 1.f, 0.f, "Refractory Period");
		configParam(RESP_PARAM, 1, 3, 2, "Responsiveness", "", 0.f);
		configParam(SENS_PARAM, 0.f, 1.f, 0.f, "Sensitivity");
		configInput(AUDIO_INPUT, "Audio In");
		configOutput(A5_OUTPUT, "A5");
		configOutput(BB5_OUTPUT, "Bb5");
		configOutput(B5_OUTPUT, "B5");
		configOutput(C5_OUTPUT, "C5");
		configOutput(DB5_OUTPUT, "Db5");
		configOutput(D5_OUTPUT, "D5");
		configOutput(EB5_OUTPUT, "Eb5");
		configOutput(E5_OUTPUT, "E5");
		configOutput(F5_OUTPUT, "F5");
		configOutput(GB5_OUTPUT, "Gb5");
		configOutput(G5_OUTPUT, "G5");
		configOutput(AB5_OUTPUT, "Ab5");
		configOutput(A4_OUTPUT, "A4");
		configOutput(BB4_OUTPUT, "Bb4");
		configOutput(B4_OUTPUT, "B4");
		configOutput(C4_OUTPUT, "C4");
		configOutput(DB4_OUTPUT, "Db4");
		configOutput(D4_OUTPUT, "D4");
		configOutput(EB4_OUTPUT, "Eb4");
		configOutput(E4_OUTPUT, "E4");
		configOutput(F4_OUTPUT, "F4");
		configOutput(GB4_OUTPUT, "Gb4");
		configOutput(G4_OUTPUT, "G4");
		configOutput(AB4_OUTPUT, "Ab4");
		configOutput(A3_OUTPUT, "A3");
		configOutput(BB3_OUTPUT, "Bb3");
		configOutput(B3_OUTPUT, "B3");
		configOutput(C3_OUTPUT, "C3");
		configOutput(DB3_OUTPUT, "Db3");
		configOutput(D3_OUTPUT, "D3");
		configOutput(EB3_OUTPUT, "Eb3");
		configOutput(E3_OUTPUT, "E3");
		configOutput(F3_OUTPUT, "F3");
		configOutput(GB3_OUTPUT, "Gb3");
		configOutput(G3_OUTPUT, "G3");
		configOutput(AB3_OUTPUT, "Ab3");
		configOutput(A2_OUTPUT, "A2");
		configOutput(BB2_OUTPUT, "Bb2");
		configOutput(B2_OUTPUT, "B2");
		configOutput(C2_OUTPUT, "C2");
		configOutput(DB2_OUTPUT, "Db2");
		configOutput(D2_OUTPUT, "D2");
		configOutput(EB2_OUTPUT, "Eb2");
		configOutput(E2_OUTPUT, "E2");
		configOutput(F2_OUTPUT, "F2");
		configOutput(GB2_OUTPUT, "Gb2");
		configOutput(G2_OUTPUT, "G2");
		configOutput(AB2_OUTPUT, "Ab2");
		configOutput(A1_OUTPUT, "A1");
		configOutput(BB1_OUTPUT, "Bb1");
		configOutput(B1_OUTPUT, "B1");
		configOutput(C1_OUTPUT, "C1");
		configOutput(DB1_OUTPUT, "Db1");
		configOutput(D1_OUTPUT, "D1");
		configOutput(EB1_OUTPUT, "Eb1");
		configOutput(E1_OUTPUT, "E1");
		configOutput(F1_OUTPUT, "F1");
		configOutput(GB1_OUTPUT, "Gb1");
		configOutput(G1_OUTPUT, "G1");
		configOutput(AB1_OUTPUT, "Ab1");
		configOutput(A0_OUTPUT, "A0");
		configOutput(BB0_OUTPUT, "Bb0");
		configOutput(B0_OUTPUT, "B0");
		configOutput(C0_OUTPUT, "C0");
		configOutput(DB0_OUTPUT, "Db0");
		configOutput(D0_OUTPUT, "D0");
		configOutput(EB0_OUTPUT, "Eb0");
		configOutput(E0_OUTPUT, "E0");
		configOutput(F0_OUTPUT, "F0");
		configOutput(GB0_OUTPUT, "Gb0");
		configOutput(G0_OUTPUT, "G0");
		configOutput(AB0_OUTPUT, "Ab0");
	}

	static void calculate_mems (float buff[buff_len]) { 
		float local_buff[buff_len];

		// Hamming window
		for (int i = 0; i < buff_len; i++)
		{
			local_buff[i] = buff[i] * hamming[i];
		}
		
		// Power spectrum
		rfft.rfft()

		// Filter Banks
		m.lock();
		//modify mems
		m.unlock();
	}

	void process(const ProcessArgs& args) override {
		buff[buff_counter++] = inputs[AUDIO_INPUT].getVoltage();

		if (buff_counter == buff_len)
		{
			buff_counter = 0;
			std::thread thred(calculate_mems, buff);
			thred.detach();
		}
		

		m.lock();
		//modify mems
		m.unlock();

	}
};


struct CochleaWidget : ModuleWidget {
	CochleaWidget(Cochlea* module) {
		setModule(module);
		setPanel(createPanel(asset::plugin(pluginInstance, "res/Cochlea.svg")));

		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
		addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
		addChild(createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));

		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(18.5, 96.0)), module, Cochlea::REF_PARAM));
		addParam(createParamCentered<RoundBlackSnapKnob>(mm2px(Vec(18.5, 106.0)), module, Cochlea::RESP_PARAM));
		addParam(createParamCentered<RoundSmallBlackKnob>(mm2px(Vec(18.5, 116.0)), module, Cochlea::SENS_PARAM));

		addInput(createInputCentered<PJ301MPort>(mm2px(Vec(6.5, 106.0)), module, Cochlea::AUDIO_INPUT));

		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 31.0)), module, Cochlea::A5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 31.0)), module, Cochlea::BB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 31.0)), module, Cochlea::B5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 31.0)), module, Cochlea::C5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 31.0)), module, Cochlea::DB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 31.0)), module, Cochlea::D5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 31.0)), module, Cochlea::EB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 31.0)), module, Cochlea::E5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 31.0)), module, Cochlea::F5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 31.0)), module, Cochlea::GB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 31.0)), module, Cochlea::G5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 31.0)), module, Cochlea::AB5_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 41.0)), module, Cochlea::A4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 41.0)), module, Cochlea::BB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 41.0)), module, Cochlea::B4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 41.0)), module, Cochlea::C4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 41.0)), module, Cochlea::DB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 41.0)), module, Cochlea::D4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 41.0)), module, Cochlea::EB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 41.0)), module, Cochlea::E4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 41.0)), module, Cochlea::F4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 41.0)), module, Cochlea::GB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 41.0)), module, Cochlea::G4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 41.0)), module, Cochlea::AB4_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 51.0)), module, Cochlea::A3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 51.0)), module, Cochlea::BB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 51.0)), module, Cochlea::B3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 51.0)), module, Cochlea::C3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 51.0)), module, Cochlea::DB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 51.0)), module, Cochlea::D3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 51.0)), module, Cochlea::EB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 51.0)), module, Cochlea::E3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 51.0)), module, Cochlea::F3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 51.0)), module, Cochlea::GB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 51.0)), module, Cochlea::G3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 51.0)), module, Cochlea::AB3_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 61.0)), module, Cochlea::A2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 61.0)), module, Cochlea::BB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 61.0)), module, Cochlea::B2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 61.0)), module, Cochlea::C2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 61.0)), module, Cochlea::DB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 61.0)), module, Cochlea::D2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 61.0)), module, Cochlea::EB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 61.0)), module, Cochlea::E2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 61.0)), module, Cochlea::F2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 61.0)), module, Cochlea::GB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 61.0)), module, Cochlea::G2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 61.0)), module, Cochlea::AB2_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 71.0)), module, Cochlea::A1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 71.0)), module, Cochlea::BB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 71.0)), module, Cochlea::B1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 71.0)), module, Cochlea::C1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 71.0)), module, Cochlea::DB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 71.0)), module, Cochlea::D1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 71.0)), module, Cochlea::EB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 71.0)), module, Cochlea::E1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 71.0)), module, Cochlea::F1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 71.0)), module, Cochlea::GB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 71.0)), module, Cochlea::G1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 71.0)), module, Cochlea::AB1_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(6.5, 81.0)), module, Cochlea::A0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(14.5, 81.0)), module, Cochlea::BB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(22.5, 81.0)), module, Cochlea::B0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(30.5, 81.0)), module, Cochlea::C0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(38.5, 81.0)), module, Cochlea::DB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(46.5, 81.0)), module, Cochlea::D0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(54.5, 81.0)), module, Cochlea::EB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(62.5, 81.0)), module, Cochlea::E0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(70.5, 81.0)), module, Cochlea::F0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(78.5, 81.0)), module, Cochlea::GB0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(86.5, 81.0)), module, Cochlea::G0_OUTPUT));
		addOutput(createOutputCentered<PJ301MPort>(mm2px(Vec(94.5, 81.0)), module, Cochlea::AB0_OUTPUT));
	}
};


Model* modelCochlea = createModel<Cochlea, CochleaWidget>("Cochlea");