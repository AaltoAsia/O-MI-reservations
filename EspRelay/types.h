#ifndef CP_TYPES_H
#define CP_TYPES_H

#include "MicroUtil.h"

typedef time_t InternalTime;

typedef String UserID;

typedef struct Reservation {
    InternalTime internalStartTime;
    InternalTime internalEndTime;
    uint32_t unixStartTime;
    uint32_t unixEndTime;
    UserID userID;
    bool active; // must be true for all unexpired reservations
} Reservation;


typedef enum AuthMethod {
    Empty,
    LocalCredentialsMD5,
    MAC,
    NFC,
    RemoteAuth
} AuthMethod;

typedef struct AuthCacheItem {
    UserID user;
    AuthMethod type;
    String authData;
} UserAuth;

#define AllReservations (int i = 0; i < DatabaseReservations; i++)
#define AllUsers (int i = 0; i < DatabaseUsers; i++)

// TODO: Non-linear search and storage
typedef struct DB {
    Reservation reservations[DatabaseReservations];
    UserAuth users[DatabaseUsers];

    void clearAllReservations() {
        for AllReservations {
            reservations[i].active = false;
        }
    };
    void clearAllUsers() {
        for AllUsers {
            users[i].type = Empty;
        }
    };

    void addReservation(Reservation& r){
        for AllReservations {
            if (!reservations[i].active) {
                reservations[i] = r;
                break;
            }
        }
    };

    void addUserAuth(UserAuth& user) {
        for AllUsers {
            if (users[i].type == Empty) {
                users[i] = user;
                break;
            }
        }
    };
} DB;

#endif
