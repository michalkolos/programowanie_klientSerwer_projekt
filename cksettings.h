#ifndef SETTINGS_H
#define SETTINGS_H




//  ## ### ### ### ##   #  #        ## ### ### ### ### ###  ##  ##
// #   #   # # #   # # # # #       #   #    #   #   #  # # #   #
// # # ##  # # ##  ##  ### #        #  ##   #   #   #  # # # #  #
// # # #   # # #   # # # # #         # #    #   #   #  # # # #   #
//  ## ### # # ### # # # # ###     ##  ###  #   #  ### # #  ## ##

#define DEFAULT_STRING_LEN      256
#define DEFAULT_BUFFER_LEN      4096
#define UNIX_TIME_COMPENSATION  2208988800





//  ##  #  ### ### ###  ## ### ###  #  ###      ## ### ### ### ### ###  ##  ##
// #   # # # # # # #   #    #   #  # # # #     #   #    #   #   #  # # #   #
// #   # # # # # # ##  #    #   #  # # # #      #  ##   #   #   #  # # # #  #
// #   # # # # # # #   #    #   #  # # # #       # #    #   #   #  # # # #   #
//  ##  #  # # # # ###  ##  #  ###  #  # #     ##  ###  #   #  ### # #  ## ##


#define TCP                 SOCK_STREAM

#define SERVER_ADDRESS          ((in_addr_t) 0x00000000)  //  INADDR_ANY
#define SERVER_PORT             9002
// how many times should the client attempt to connect to a server
#define MAX_CONNECT_ATTEMPTS    5







// #    #   ##  ## ### ###  ##      ## ### ### ### ### ###  ##  ##
// #   # # #   #    #  # # #       #   #    #   #   #  # # #   #
// #   # # # # # #  #  # # # #      #  ##   #   #   #  # # # #  #
// #   # # # # # #  #  # # # #       # #    #   #   #  # # # #   #
// ###  #   ##  ## ### # #  ##     ##  ###  #   #  ### # #  ## ##

//path to the logfile
#define LOG_FILE "log.txt"
//log destinations
#define TO_CONSOLE  1
#define TO_FILE     0
//messages with log level lower than this will be logged (0 - all messages)
#define LOG_FILTER  0



#endif
