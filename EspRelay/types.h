

typedef time_t InternalTime;



typedef struct Reservation {
    InternalTime internalStartTime;
    InternalTime internalEndTime;
    uint32_t unixStartTime;
    uint32_t unixEndTime;
    UserID userID;
} Reservation;


typedef enum AuthMethod {
    NFC,
    LocalCredentials,
    RemoteAuth
}

typedef struct AuthCache {
    AuthMethod type;
    UserID user;
}

