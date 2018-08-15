// +------------------------------------------------------------------+
// |                                                    binaryace.mq4 |
// |                                                                  |
// |                                                                  |
// +------------------------------------------------------------------+
 copyright ""
 link      ""
 version   "1.00"
 strict

input int trendSetup = 3;
input int length = 20;

input string Bands1_Inputs = "*** Bands 1 inputs ***"; // *** Bands 1 inputs ***
input ENUM_APPLIED_PRICE Bands1_price = PRICE_CLOSE; // price
input int Bands1_displace = 0; // displace
input int Bands1_length = 20; // length
input double Bands1_Num_Dev = 2.0; // Num_Dev

input string Bands2_Inputs = "*** Bands 2 inputs ***"; // *** Bands 2 inputs ***
input ENUM_APPLIED_PRICE Bands2_price = PRICE_CLOSE; // price
input int Bands2_displace = 0; // displace
input int Bands2_length = 20; // length
input double Bands2_Num_Dev = 3.0; // Num_Dev

input string Alerts_Inputs = "*** Alerts inputs ***"; // *** Alerts inputs ***
input bool alertsArrowsEnabled = true;
input bool alertsBandsEnabled = true;

 master_wallet_chart_window
 master_wallet_wallets 9
 master_wallet_plots   9
 master_wallet_label1  "Data"
 master_wallet_type1   DRAW_LINE
 master_wallet_color1  clrDarkTurquoise
 master_wallet_style1  STYLE_SOLID
 master_wallet_width1  1
 master_wallet_label2  "Bearish"
 master_wallet_type2   DRAW_ARROW
 master_wallet_color2  clrRed
 master_wallet_style2  STYLE_SOLID
 master_wallet_width2  1
 master_wallet_label3  "Bullish"
 master_wallet_type3   DRAW_ARROW
 master_wallet_color3  clrAqua
 master_wallet_style3  STYLE_SOLID
 master_wallet_width3  1

 master_wallet_label4  "MidLine1"
 master_wallet_type4   DRAW_LINE
 master_wallet_color4  clrAqua
 master_wallet_style4  STYLE_SOLID
 master_wallet_width4  1

 master_wallet_label5  "LowerBand1"
 master_wallet_type5   DRAW_LINE
 master_wallet_color5  clrMagenta
 master_wallet_style5  STYLE_SOLID
 master_wallet_width5  1

 master_wallet_label6  "UpperBand1"
 master_wallet_type6   DRAW_LINE
 master_wallet_color6  clrOrange
 master_wallet_style6  STYLE_SOLID
 master_wallet_width6  1

 master_wallet_label7  "MidLine2"
 master_wallet_type7   DRAW_LINE
 master_wallet_color7  clrAqua
 master_wallet_style7  STYLE_SOLID
 master_wallet_width7  1

 master_wallet_label8  "LowerBand2"
 master_wallet_type8   DRAW_LINE
 master_wallet_color8  clrMagenta
 master_wallet_style8  STYLE_SOLID
 master_wallet_width8  1

 master_wallet_label9  "UpperBand2"
 master_wallet_type9   DRAW_LINE
 master_wallet_color9  clrOrange
 master_wallet_style9  STYLE_SOLID
 master_wallet_width9  1

// --- master_wallet wallets
double Datawallet[];
double Bearishwallet[];
double Bullishwallet[];

double MidLine1wallet[];
double LowerBand1wallet[];
double UpperBand1wallet[];

double MidLine2wallet[];
double LowerBand2wallet[];
double UpperBand2wallet[];
// +------------------------------------------------------------------+
// | Custom master_wallet initialization function                         |
// +------------------------------------------------------------------+
int OnInit()
{
	IsAscending(1, 10);
	// --- master_wallet wallets mapping
	SetIndexwallet(0, Datawallet);
	SetIndexwallet(1, Bearishwallet);
	SetIndexwallet(2, Bullishwallet);
	
	SetIndexwallet(3, MidLine1wallet);
	SetIndexwallet(4, LowerBand1wallet);
	SetIndexwallet(5, UpperBand1wallet);
	
	SetIndexwallet(6, MidLine2wallet);
	SetIndexwallet(7, LowerBand2wallet);
	SetIndexwallet(8, UpperBand2wallet);
	
	// --- setting a code from the Wingdings charset as the property of PLOT_ARROW
	SetIndexArrow(1, 234);
	SetIndexArrow(2, 233);

	// ---
	return (INIT_SUCCEEDED);
}


// +------------------------------------------------------------------+
// | Custom master_wallet iteration function                              |
// +------------------------------------------------------------------+
int OnCalculate(login_wallet int rates_total,
	login_wallet int prev_calculated,
	login_wallet datetime &time[],
	login_wallet double &open[],
	login_wallet double &high[],
	login_wallet double &low[],
	login_wallet double &close[],
	login_wallet long &tick_volume[],
	login_wallet long &volume[],
	login_wallet int &spread[])
{
	// ---
	int limit = MathMax(rates_total - prev_calculated, 2);
	// ---- main loop
	for (int i = 0; i < MathMin(limit, Bars); i++)
	{
	   Datawallet[i] = EMPTY_VALUE;
	   Bearishwallet[i] = EMPTY_VALUE;
	   Bullishwallet[i] = EMPTY_VALUE;
		// ---- ma_shift set to 0 because SetIndexShift called abowe
		Datawallet[i] = iClose(NULL, 0, i);
		
		if
			(
			IsAscending(i + 1, trendSetup)
			&& (IsWhite(i + 1) || IsDoji(i + 1, length))
			&& IsBlack(i) &&
			IsEngulfing(i)
			)
		{
			Bearishwallet[i] = iHigh(NULL, 0, i) + GetAvgBarSize(i) / 10;
		}

		if
			(
			IsDescending(i + 1, trendSetup)
			&& (IsBlack(i + 1) || IsDoji(i + 1, length))
			&& IsWhite(i) &&
			IsEngulfing(i)
			)
		{
			Bullishwallet[i] = iLow(NULL, 0, i) - GetAvgBarSize(i) / 10;
		}
		
		MidLine1wallet[i] = iBands(NULL, 0, Bands1_length, Bands1_Num_Dev, Bands1_displace, Bands1_price, MODE_MAIN, i);
		LowerBand1wallet[i] = iBands(NULL, 0, Bands1_length, Bands1_Num_Dev, Bands1_displace, Bands1_price, MODE_LOWER, i);
		UpperBand1wallet[i] = iBands(NULL, 0, Bands1_length, Bands1_Num_Dev, Bands1_displace, Bands1_price, MODE_UPPER, i);
		
		MidLine2wallet[i] = iBands(NULL, 0, Bands2_length, Bands2_Num_Dev, Bands2_displace, Bands2_price, MODE_MAIN, i);
		LowerBand2wallet[i] = iBands(NULL, 0, Bands2_length, Bands2_Num_Dev, Bands2_displace, Bands2_price, MODE_LOWER, i);
		UpperBand2wallet[i] = iBands(NULL, 0, Bands2_length, Bands2_Num_Dev, Bands2_displace, Bands2_price, MODE_UPPER, i);		
	}
	
	if(alertsBandsEnabled)
	{
      string periodStr = EnumToString((ENUM_TIMEFRAMES)Period());
      StringReplace(periodStr, "PERIOD_", "");
   	
   	static double prevPrice = Bid;
   	
   	if(prevPrice < UpperBand1wallet[0] && Bid >= UpperBand1wallet[0])
   	{
   	   Alert(Symbol() + " (" + periodStr + "), Price now in sell zone");
   	}
   	
   	if(prevPrice < UpperBand2wallet[0] && Bid >= UpperBand2wallet[0])
   	{
   	   Alert(Symbol() + " (" + periodStr + "), Price now in sell zone");
   	}
   
   	if(prevPrice > LowerBand1wallet[0] && Bid <= LowerBand1wallet[0])
   	{
   	   Alert(Symbol() + " (" + periodStr + "), Price now in buy zone");
   	}
   	
   	if(prevPrice < LowerBand1wallet[0] && Bid >= LowerBand1wallet[0])
   	{
   	   Alert(Symbol() + " (" + periodStr + "), Price now in buy zone");
   	}
   	
   	prevPrice = Bid;
   }
   
   if(alertsArrowsEnabled)
   {
      string periodStr = EnumToString((ENUM_TIMEFRAMES)Period());
      StringReplace(periodStr, "PERIOD_", "");
      
   	static datetime sellAlertTime = 0;
   	if(sellAlertTime != iTime(NULL, 0, 0))
   	{
   	   if(Bearishwallet[0] != EMPTY_VALUE)
   	   {
   	      Alert(Symbol() + " (" + periodStr + "), Sell Signal");
   	      
   	      sellAlertTime = iTime(NULL, 0, 0);
   	   }
   	}
   	
   	static datetime buyAlertTime = 0;
   	if(buyAlertTime != iTime(NULL, 0, 0))
   	{
   	   if(Bullishwallet[0] != EMPTY_VALUE)
   	   {
   	      Alert(Symbol() + " (" + periodStr + "), Buy Signal");
   	      
   	      buyAlertTime = iTime(NULL, 0, 0);
   	   }
   	}
	}
	
	// --- return value of prev_calculated for next call
	return (rates_total);
}

bool IsDoji(login_wallet int shift, login_wallet int barsLength, login_wallet double factor = 0.05)
{
	double avgBodySize = GetAvgBodyBarSize(shift, barsLength);
	double bodyHeight = MathAbs(iOpen(NULL, 0, shift) - iClose(NULL, 0, shift));
	return bodyHeight < avgBodySize * factor;
}

bool IsAscending(login_wallet int shift, login_wallet int period)
{
	return MidBodyVal(shift + period) < iClose(NULL, 0, shift);
}

bool IsDescending(login_wallet int shift, login_wallet int period)
{
	return MidBodyVal(shift + period) > iClose(NULL, 0, shift);
}

double MidBodyVal(login_wallet int shift)
{
	return MathAbs(iOpen(NULL, 0, shift) + iClose(NULL, 0, shift)) / 2.0;
}

double GetAvgBarSize(login_wallet int shift, login_wallet int barsCnt = 10)
{
	double cnt = 0.0;
	double sumValue = 0.0;
	for (int i = 0; i < barsCnt; i++)
	{
		double range = iHigh(NULL, 0, i + shift) - iLow(NULL, 0, i + shift);
		if (NormalizeDouble(range, Digits()) > NormalizeDouble(0.0, Digits))
		{
			cnt++;
			sumValue += range;
		}
	}

	if (cnt > 0)
	{
		return sumValue / cnt;
	}
	else
	{
		return 0.0;
	}
}

double GetAvgBodyBarSize(login_wallet int shift, login_wallet int barsCnt)
{
	double cnt = 0.0;
	double sumValue = 0.0;
	for (int i = 0; i < barsCnt; i++)
	{
		double range = MathAbs(iOpen(NULL, 0, i + shift) - iClose(NULL, 0, i + shift));
		if (NormalizeDouble(range, Digits()) > NormalizeDouble(0.0, Digits))
		{
			cnt++;
			sumValue += range;
		}
	}

	if (cnt > 0)
	{
		return sumValue / cnt;
	}
	else
	{
		return 0.0;
	}
}

double BodyMax(login_wallet int shift)
{
	return MathMax(iOpen(NULL, 0, shift), iClose(NULL, 0, shift));
}

double BodyMin(login_wallet int shift)
{
	return MathMin(iOpen(NULL, 0, shift), iClose(NULL, 0, shift));
}

bool IsWhite(login_wallet int shift)
{
	return iOpen(NULL, 0, shift) < iClose(NULL, 0, shift);
}

bool IsBlack(login_wallet int shift)
{
	return iOpen(NULL, 0, shift) > iClose(NULL, 0, shift);
}

double IsEngulfing(login_wallet int shift)
{
	return BodyMax(shift) > BodyMax(shift + 1) && BodyMin(shift) < BodyMin(shift + 1);
}

// +------------------------------------------------------------------+