
#include "MicroUtil.h"
#include "ChargingPoleOMI.h"
#include "Config.h"
#include "OMIProcessing.h"

char * readUpdate(const char* lastUpdateTimestamp){
    //return sprintf_P(_msgBufL, readBeginTemplate, lastUpdateTimestamp);
    return strcpy_P(_msgBufL, readBeginTemplate);
};

#define GetValueOfItem(name, object, value) \
                tmp = 0; tmpb = 0; \
                if (!(findInfoItem((object), (name), item, tmp) \
                            && findValue(item, (value), tmpb))) { \
                    D("[OMIreserve] Error: Cannot parse "); DS(name); \
                    continue; \
                }

uint64_t char2LL(String str);
uint64_t char2LL(String str)
{
  uint64_t result = 0; // Initialize result
  // Iterate through all characters of input string and update result
  for (int i = 0; i < str.length(); ++i)
    result = result*10 + str[i] - '0';
  return result;
}

void processReservations(String& omiResponse, DB& database) {
    unsigned tmp = 0, tmpb = 0;

    String reservationsXml;
    if (findObject(omiResponse, ReservationsObjectId, reservationsXml, tmp)) {
        String reservationObject;
        unsigned processed = 25; // Reservation object length ~ 30

        // TODO: update only changes to db
        database.clearAllReservations();

        while (processed < reservationsXml.length()) {
            D("PROCESSED = "); DNUM(processed);
            if (findObject(reservationsXml, String(), reservationObject, processed)){
                String item;
                String value;
                Reservation reservation = {0};

                GetValueOfItem(UserIDId, reservationObject, value)
                reservation.userID = value;

                GetValueOfItem(StartsAfterName, reservationObject, value)
                DSLN(value);
                reservation.unixStartTime = char2LL(value);
                DSLN(reservation.unixStartTime);

                //reservation.internalStartTime = getCurrentTime() + parseInt(value);

                GetValueOfItem(EndsAfterName, reservationObject, value)
                //reservation.internalEndTime = getCurrentTime() + parseInt(value);
                reservation.unixEndTime = char2LL(value);

                // TODO: other items

                DLN("[OMIreserve] PARSED RESERVATION SUCCESFULLY");
                DNUM(reservation.unixStartTime);
                D(" ");
                DNUM(reservation.unixEndTime);
                reservation.active = true;
                database.addReservation(reservation);
            }
        }
    }
}

#define SaveUserAuthData(name, typee) \
                tmp = 0; tmpb = 0; \
                if (findInfoItem(userObject, (name), item, tmp) \
                            && findValue(item, value, tmpb)) { \
                    userAuth.type = (typee); \
                    userAuth.authData = value; \
                    database.addUserAuth(userAuth); \
                }

void processUsers(String& omiResponse, DB& database) {
    unsigned tmp = 0, tmpb = 0;

    String usersXml;
    if (findObject(omiResponse, UsersObjectId, usersXml, tmp)) {
        String userObject;
        unsigned processed = 20;

        // TODO: update only changes to db
        database.clearAllUsers();

        while (processed < usersXml.length()) {
            if (findObject(usersXml, String(), userObject, processed)){
                String user;
                String item;
                String value;
                UserAuth userAuth;
                userAuth.type = Empty;

                GetValueOfItem(UserIDId, userObject, user)

                userAuth.user = user;

                tmp = 0;
                if (!findObject(userObject, AuthId, userObject, tmp)) {
                    DLN("[OMIauth] Error: no Auth object");
                    continue;
                };

                SaveUserAuthData(NFCName, NFC)

                SaveUserAuthData(MACName, MAC)

                SaveUserAuthData(PassName, LocalCredentialsMD5)
            }
        }
    }
}

