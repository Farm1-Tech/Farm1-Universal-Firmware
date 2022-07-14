
enum RTC_Type {
    DS1338 = 0x68,
    MCP79411 = 0x6F,
    PCF8563 = 0x51,
};

bool RTC_init() ;
bool RTC_read(struct tm* timeinfo) ;
bool RTC_write(struct tm* timeinfo) ;

