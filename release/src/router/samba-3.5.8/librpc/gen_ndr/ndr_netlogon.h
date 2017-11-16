/* header auto-generated by pidl */

#include "librpc/ndr/libndr.h"
#include "../librpc/gen_ndr/netlogon.h"

#ifndef _HEADER_NDR_netlogon
#define _HEADER_NDR_netlogon

#include "../librpc/ndr/ndr_netlogon.h"
#define NDR_NETLOGON_UUID "12345678-1234-abcd-ef00-01234567cffb"
#define NDR_NETLOGON_VERSION 1.0
#define NDR_NETLOGON_NAME "netlogon"
#define NDR_NETLOGON_HELPSTRING NULL
extern const struct ndr_interface_table ndr_table_netlogon;
#define NDR_NETR_LOGONUASLOGON (0x00)

#define NDR_NETR_LOGONUASLOGOFF (0x01)

#define NDR_NETR_LOGONSAMLOGON (0x02)

#define NDR_NETR_LOGONSAMLOGOFF (0x03)

#define NDR_NETR_SERVERREQCHALLENGE (0x04)

#define NDR_NETR_SERVERAUTHENTICATE (0x05)

#define NDR_NETR_SERVERPASSWORDSET (0x06)

#define NDR_NETR_DATABASEDELTAS (0x07)

#define NDR_NETR_DATABASESYNC (0x08)

#define NDR_NETR_ACCOUNTDELTAS (0x09)

#define NDR_NETR_ACCOUNTSYNC (0x0a)

#define NDR_NETR_GETDCNAME (0x0b)

#define NDR_NETR_LOGONCONTROL (0x0c)

#define NDR_NETR_GETANYDCNAME (0x0d)

#define NDR_NETR_LOGONCONTROL2 (0x0e)

#define NDR_NETR_SERVERAUTHENTICATE2 (0x0f)

#define NDR_NETR_DATABASESYNC2 (0x10)

#define NDR_NETR_DATABASEREDO (0x11)

#define NDR_NETR_LOGONCONTROL2EX (0x12)

#define NDR_NETR_NETRENUMERATETRUSTEDDOMAINS (0x13)

#define NDR_NETR_DSRGETDCNAME (0x14)

#define NDR_NETR_LOGONGETCAPABILITIES (0x15)

#define NDR_NETR_NETRLOGONSETSERVICEBITS (0x16)

#define NDR_NETR_LOGONGETTRUSTRID (0x17)

#define NDR_NETR_NETRLOGONCOMPUTESERVERDIGEST (0x18)

#define NDR_NETR_NETRLOGONCOMPUTECLIENTDIGEST (0x19)

#define NDR_NETR_SERVERAUTHENTICATE3 (0x1a)

#define NDR_NETR_DSRGETDCNAMEEX (0x1b)

#define NDR_NETR_DSRGETSITENAME (0x1c)

#define NDR_NETR_LOGONGETDOMAININFO (0x1d)

#define NDR_NETR_SERVERPASSWORDSET2 (0x1e)

#define NDR_NETR_SERVERPASSWORDGET (0x1f)

#define NDR_NETR_NETRLOGONSENDTOSAM (0x20)

#define NDR_NETR_DSRADDRESSTOSITENAMESW (0x21)

#define NDR_NETR_DSRGETDCNAMEEX2 (0x22)

#define NDR_NETR_NETRLOGONGETTIMESERVICEPARENTDOMAIN (0x23)

#define NDR_NETR_NETRENUMERATETRUSTEDDOMAINSEX (0x24)

#define NDR_NETR_DSRADDRESSTOSITENAMESEXW (0x25)

#define NDR_NETR_DSRGETDCSITECOVERAGEW (0x26)

#define NDR_NETR_LOGONSAMLOGONEX (0x27)

#define NDR_NETR_DSRENUMERATEDOMAINTRUSTS (0x28)

#define NDR_NETR_DSRDEREGISTERDNSHOSTRECORDS (0x29)

#define NDR_NETR_SERVERTRUSTPASSWORDSGET (0x2a)

#define NDR_NETR_DSRGETFORESTTRUSTINFORMATION (0x2b)

#define NDR_NETR_GETFORESTTRUSTINFORMATION (0x2c)

#define NDR_NETR_LOGONSAMLOGONWITHFLAGS (0x2d)

#define NDR_NETR_SERVERGETTRUSTINFO (0x2e)

#define NDR_NETLOGON_CALL_COUNT (47)
void ndr_print_netr_UasInfo(struct ndr_print *ndr, const char *name, const struct netr_UasInfo *r);
void ndr_print_netr_UasLogoffInfo(struct ndr_print *ndr, const char *name, const struct netr_UasLogoffInfo *r);
enum ndr_err_code ndr_push_netr_AcctLockStr(struct ndr_push *ndr, int ndr_flags, const struct netr_AcctLockStr *r);
enum ndr_err_code ndr_pull_netr_AcctLockStr(struct ndr_pull *ndr, int ndr_flags, struct netr_AcctLockStr *r);
void ndr_print_netr_AcctLockStr(struct ndr_print *ndr, const char *name, const struct netr_AcctLockStr *r);
enum ndr_err_code ndr_push_netr_LogonParameterControl(struct ndr_push *ndr, int ndr_flags, uint32_t r);
enum ndr_err_code ndr_pull_netr_LogonParameterControl(struct ndr_pull *ndr, int ndr_flags, uint32_t *r);
void ndr_print_netr_LogonParameterControl(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_IdentityInfo(struct ndr_print *ndr, const char *name, const struct netr_IdentityInfo *r);
void ndr_print_netr_PasswordInfo(struct ndr_print *ndr, const char *name, const struct netr_PasswordInfo *r);
void ndr_print_netr_ChallengeResponse(struct ndr_print *ndr, const char *name, const struct netr_ChallengeResponse *r);
void ndr_print_netr_NetworkInfo(struct ndr_print *ndr, const char *name, const struct netr_NetworkInfo *r);
void ndr_print_netr_GenericInfo(struct ndr_print *ndr, const char *name, const struct netr_GenericInfo *r);
void ndr_print_netr_LogonInfoClass(struct ndr_print *ndr, const char *name, enum netr_LogonInfoClass r);
enum ndr_err_code ndr_push_netr_LogonLevel(struct ndr_push *ndr, int ndr_flags, const union netr_LogonLevel *r);
enum ndr_err_code ndr_pull_netr_LogonLevel(struct ndr_pull *ndr, int ndr_flags, union netr_LogonLevel *r);
void ndr_print_netr_LogonLevel(struct ndr_print *ndr, const char *name, const union netr_LogonLevel *r);
enum ndr_err_code ndr_push_netr_UserSessionKey(struct ndr_push *ndr, int ndr_flags, const struct netr_UserSessionKey *r);
enum ndr_err_code ndr_pull_netr_UserSessionKey(struct ndr_pull *ndr, int ndr_flags, struct netr_UserSessionKey *r);
void ndr_print_netr_UserSessionKey(struct ndr_print *ndr, const char *name, const struct netr_UserSessionKey *r);
enum ndr_err_code ndr_push_netr_LMSessionKey(struct ndr_push *ndr, int ndr_flags, const struct netr_LMSessionKey *r);
enum ndr_err_code ndr_pull_netr_LMSessionKey(struct ndr_pull *ndr, int ndr_flags, struct netr_LMSessionKey *r);
void ndr_print_netr_LMSessionKey(struct ndr_print *ndr, const char *name, const struct netr_LMSessionKey *r);
enum ndr_err_code ndr_push_netr_UserFlags(struct ndr_push *ndr, int ndr_flags, uint32_t r);
enum ndr_err_code ndr_pull_netr_UserFlags(struct ndr_pull *ndr, int ndr_flags, uint32_t *r);
void ndr_print_netr_UserFlags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_SamBaseInfo(struct ndr_print *ndr, const char *name, const struct netr_SamBaseInfo *r);
void ndr_print_netr_SamInfo2(struct ndr_print *ndr, const char *name, const struct netr_SamInfo2 *r);
void ndr_print_netr_SidAttr(struct ndr_print *ndr, const char *name, const struct netr_SidAttr *r);
enum ndr_err_code ndr_push_netr_SamInfo3(struct ndr_push *ndr, int ndr_flags, const struct netr_SamInfo3 *r);
enum ndr_err_code ndr_pull_netr_SamInfo3(struct ndr_pull *ndr, int ndr_flags, struct netr_SamInfo3 *r);
void ndr_print_netr_SamInfo3(struct ndr_print *ndr, const char *name, const struct netr_SamInfo3 *r);
void ndr_print_netr_SamInfo6(struct ndr_print *ndr, const char *name, const struct netr_SamInfo6 *r);
void ndr_print_netr_PacInfo(struct ndr_print *ndr, const char *name, const struct netr_PacInfo *r);
void ndr_print_netr_GenericInfo2(struct ndr_print *ndr, const char *name, const struct netr_GenericInfo2 *r);
enum ndr_err_code ndr_push_netr_Validation(struct ndr_push *ndr, int ndr_flags, const union netr_Validation *r);
enum ndr_err_code ndr_pull_netr_Validation(struct ndr_pull *ndr, int ndr_flags, union netr_Validation *r);
void ndr_print_netr_Validation(struct ndr_print *ndr, const char *name, const union netr_Validation *r);
enum ndr_err_code ndr_push_netr_Credential(struct ndr_push *ndr, int ndr_flags, const struct netr_Credential *r);
enum ndr_err_code ndr_pull_netr_Credential(struct ndr_pull *ndr, int ndr_flags, struct netr_Credential *r);
void ndr_print_netr_Credential(struct ndr_print *ndr, const char *name, const struct netr_Credential *r);
enum ndr_err_code ndr_push_netr_Authenticator(struct ndr_push *ndr, int ndr_flags, const struct netr_Authenticator *r);
enum ndr_err_code ndr_pull_netr_Authenticator(struct ndr_pull *ndr, int ndr_flags, struct netr_Authenticator *r);
void ndr_print_netr_Authenticator(struct ndr_print *ndr, const char *name, const struct netr_Authenticator *r);
void ndr_print_netr_DELTA_DELETE_USER(struct ndr_print *ndr, const char *name, const struct netr_DELTA_DELETE_USER *r);
void ndr_print_netr_USER_KEY16(struct ndr_print *ndr, const char *name, const struct netr_USER_KEY16 *r);
void ndr_print_netr_PasswordHistory(struct ndr_print *ndr, const char *name, const struct netr_PasswordHistory *r);
void ndr_print_netr_USER_KEYS2(struct ndr_print *ndr, const char *name, const struct netr_USER_KEYS2 *r);
void ndr_print_netr_USER_KEY_UNION(struct ndr_print *ndr, const char *name, const struct netr_USER_KEY_UNION *r);
enum ndr_err_code ndr_push_netr_USER_KEYS(struct ndr_push *ndr, int ndr_flags, const struct netr_USER_KEYS *r);
enum ndr_err_code ndr_pull_netr_USER_KEYS(struct ndr_pull *ndr, int ndr_flags, struct netr_USER_KEYS *r);
void ndr_print_netr_USER_KEYS(struct ndr_print *ndr, const char *name, const struct netr_USER_KEYS *r);
void ndr_print_netr_USER_PRIVATE_INFO(struct ndr_print *ndr, const char *name, const struct netr_USER_PRIVATE_INFO *r);
void ndr_print_netr_DELTA_USER(struct ndr_print *ndr, const char *name, const struct netr_DELTA_USER *r);
void ndr_print_netr_DELTA_DOMAIN(struct ndr_print *ndr, const char *name, const struct netr_DELTA_DOMAIN *r);
void ndr_print_netr_DELTA_GROUP(struct ndr_print *ndr, const char *name, const struct netr_DELTA_GROUP *r);
void ndr_print_netr_DELTA_RENAME(struct ndr_print *ndr, const char *name, const struct netr_DELTA_RENAME *r);
void ndr_print_netr_DELTA_GROUP_MEMBER(struct ndr_print *ndr, const char *name, const struct netr_DELTA_GROUP_MEMBER *r);
void ndr_print_netr_DELTA_ALIAS(struct ndr_print *ndr, const char *name, const struct netr_DELTA_ALIAS *r);
void ndr_print_netr_DELTA_ALIAS_MEMBER(struct ndr_print *ndr, const char *name, const struct netr_DELTA_ALIAS_MEMBER *r);
void ndr_print_netr_QUOTA_LIMITS(struct ndr_print *ndr, const char *name, const struct netr_QUOTA_LIMITS *r);
void ndr_print_netr_DELTA_POLICY(struct ndr_print *ndr, const char *name, const struct netr_DELTA_POLICY *r);
void ndr_print_netr_DELTA_TRUSTED_DOMAIN(struct ndr_print *ndr, const char *name, const struct netr_DELTA_TRUSTED_DOMAIN *r);
void ndr_print_netr_DELTA_DELETE_TRUST(struct ndr_print *ndr, const char *name, const struct netr_DELTA_DELETE_TRUST *r);
void ndr_print_netr_DELTA_ACCOUNT(struct ndr_print *ndr, const char *name, const struct netr_DELTA_ACCOUNT *r);
void ndr_print_netr_DELTA_DELETE_ACCOUNT(struct ndr_print *ndr, const char *name, const struct netr_DELTA_DELETE_ACCOUNT *r);
void ndr_print_netr_DELTA_DELETE_SECRET(struct ndr_print *ndr, const char *name, const struct netr_DELTA_DELETE_SECRET *r);
void ndr_print_netr_CIPHER_VALUE(struct ndr_print *ndr, const char *name, const struct netr_CIPHER_VALUE *r);
void ndr_print_netr_DELTA_SECRET(struct ndr_print *ndr, const char *name, const struct netr_DELTA_SECRET *r);
void ndr_print_netr_DeltaEnum(struct ndr_print *ndr, const char *name, enum netr_DeltaEnum r);
void ndr_print_netr_DELTA_UNION(struct ndr_print *ndr, const char *name, const union netr_DELTA_UNION *r);
void ndr_print_netr_DELTA_ID_UNION(struct ndr_print *ndr, const char *name, const union netr_DELTA_ID_UNION *r);
void ndr_print_netr_DELTA_ENUM(struct ndr_print *ndr, const char *name, const struct netr_DELTA_ENUM *r);
void ndr_print_netr_DELTA_ENUM_ARRAY(struct ndr_print *ndr, const char *name, const struct netr_DELTA_ENUM_ARRAY *r);
void ndr_print_netr_UAS_INFO_0(struct ndr_print *ndr, const char *name, const struct netr_UAS_INFO_0 *r);
void ndr_print_netr_AccountBuffer(struct ndr_print *ndr, const char *name, const struct netr_AccountBuffer *r);
void ndr_print_netr_InfoFlags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_NETLOGON_INFO_1(struct ndr_print *ndr, const char *name, const struct netr_NETLOGON_INFO_1 *r);
void ndr_print_netr_NETLOGON_INFO_2(struct ndr_print *ndr, const char *name, const struct netr_NETLOGON_INFO_2 *r);
void ndr_print_netr_NETLOGON_INFO_3(struct ndr_print *ndr, const char *name, const struct netr_NETLOGON_INFO_3 *r);
void ndr_print_netr_NETLOGON_INFO_4(struct ndr_print *ndr, const char *name, const struct netr_NETLOGON_INFO_4 *r);
void ndr_print_netr_CONTROL_QUERY_INFORMATION(struct ndr_print *ndr, const char *name, const union netr_CONTROL_QUERY_INFORMATION *r);
void ndr_print_netr_LogonControlCode(struct ndr_print *ndr, const char *name, enum netr_LogonControlCode r);
void ndr_print_netr_CONTROL_DATA_INFORMATION(struct ndr_print *ndr, const char *name, const union netr_CONTROL_DATA_INFORMATION *r);
enum ndr_err_code ndr_push_netr_NegotiateFlags(struct ndr_push *ndr, int ndr_flags, uint32_t r);
enum ndr_err_code ndr_pull_netr_NegotiateFlags(struct ndr_pull *ndr, int ndr_flags, uint32_t *r);
void ndr_print_netr_NegotiateFlags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_SyncStateEnum(struct ndr_print *ndr, const char *name, enum SyncStateEnum r);
void ndr_print_netr_ChangeLogFlags(struct ndr_print *ndr, const char *name, uint16_t r);
void ndr_print_netr_ChangeLogObject(struct ndr_print *ndr, const char *name, const union netr_ChangeLogObject *r);
enum ndr_err_code ndr_push_netr_ChangeLogEntry(struct ndr_push *ndr, int ndr_flags, const struct netr_ChangeLogEntry *r);
enum ndr_err_code ndr_pull_netr_ChangeLogEntry(struct ndr_pull *ndr, int ndr_flags, struct netr_ChangeLogEntry *r);
void ndr_print_netr_ChangeLogEntry(struct ndr_print *ndr, const char *name, const struct netr_ChangeLogEntry *r);
size_t ndr_size_netr_ChangeLogEntry(const struct netr_ChangeLogEntry *r, struct smb_iconv_convenience *ic, int flags);
void ndr_print_netr_Blob(struct ndr_print *ndr, const char *name, const struct netr_Blob *r);
void ndr_print_netr_DsRGetDCName_flags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_DsRGetDCNameInfo_AddressType(struct ndr_print *ndr, const char *name, enum netr_DsRGetDCNameInfo_AddressType r);
void ndr_print_netr_DsR_DcFlags(struct ndr_print *ndr, const char *name, uint32_t r);
enum ndr_err_code ndr_push_netr_DsRGetDCNameInfo(struct ndr_push *ndr, int ndr_flags, const struct netr_DsRGetDCNameInfo *r);
enum ndr_err_code ndr_pull_netr_DsRGetDCNameInfo(struct ndr_pull *ndr, int ndr_flags, struct netr_DsRGetDCNameInfo *r);
void ndr_print_netr_DsRGetDCNameInfo(struct ndr_print *ndr, const char *name, const struct netr_DsRGetDCNameInfo *r);
void ndr_print_netr_Capabilities(struct ndr_print *ndr, const char *name, const union netr_Capabilities *r);
void ndr_print_netr_TrustFlags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_WorkstationFlags(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_SuiteMask(struct ndr_print *ndr, const char *name, uint16_t r);
void ndr_print_netr_ProductType(struct ndr_print *ndr, const char *name, uint8_t r);
void ndr_print_netr_LsaPolicyInformation(struct ndr_print *ndr, const char *name, const struct netr_LsaPolicyInformation *r);
void ndr_print_netr_OsVersionInfoEx(struct ndr_print *ndr, const char *name, const struct netr_OsVersionInfoEx *r);
void ndr_print_netr_OsVersion(struct ndr_print *ndr, const char *name, const struct netr_OsVersion *r);
void ndr_print_netr_OsVersionContainer(struct ndr_print *ndr, const char *name, const struct netr_OsVersionContainer *r);
void ndr_print_netr_WorkstationInformation(struct ndr_print *ndr, const char *name, const struct netr_WorkstationInformation *r);
void ndr_print_netr_WorkstationInfo(struct ndr_print *ndr, const char *name, const union netr_WorkstationInfo *r);
void ndr_print_netr_trust_extension(struct ndr_print *ndr, const char *name, const struct netr_trust_extension *r);
void ndr_print_netr_trust_extension_container(struct ndr_print *ndr, const char *name, const struct netr_trust_extension_container *r);
void ndr_print_netr_OneDomainInfo(struct ndr_print *ndr, const char *name, const struct netr_OneDomainInfo *r);
enum ndr_err_code ndr_push_netr_SupportedEncTypes(struct ndr_push *ndr, int ndr_flags, uint32_t r);
enum ndr_err_code ndr_pull_netr_SupportedEncTypes(struct ndr_pull *ndr, int ndr_flags, uint32_t *r);
void ndr_print_netr_SupportedEncTypes(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_DomainInformation(struct ndr_print *ndr, const char *name, const struct netr_DomainInformation *r);
void ndr_print_netr_DomainInfo(struct ndr_print *ndr, const char *name, const union netr_DomainInfo *r);
void ndr_print_netr_CryptPassword(struct ndr_print *ndr, const char *name, const struct netr_CryptPassword *r);
void ndr_print_netr_DsRAddressToSitenamesWCtr(struct ndr_print *ndr, const char *name, const struct netr_DsRAddressToSitenamesWCtr *r);
void ndr_print_netr_DsRAddress(struct ndr_print *ndr, const char *name, const struct netr_DsRAddress *r);
void ndr_print_netr_TrustType(struct ndr_print *ndr, const char *name, enum netr_TrustType r);
void ndr_print_netr_TrustAttributes(struct ndr_print *ndr, const char *name, uint32_t r);
void ndr_print_netr_DomainTrust(struct ndr_print *ndr, const char *name, const struct netr_DomainTrust *r);
void ndr_print_netr_DomainTrustList(struct ndr_print *ndr, const char *name, const struct netr_DomainTrustList *r);
void ndr_print_netr_DsRAddressToSitenamesExWCtr(struct ndr_print *ndr, const char *name, const struct netr_DsRAddressToSitenamesExWCtr *r);
void ndr_print_DcSitesCtr(struct ndr_print *ndr, const char *name, const struct DcSitesCtr *r);
void ndr_print_netr_TrustInfo(struct ndr_print *ndr, const char *name, const struct netr_TrustInfo *r);
void ndr_print_netr_LogonUasLogon(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonUasLogon *r);
void ndr_print_netr_LogonUasLogoff(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonUasLogoff *r);
void ndr_print_netr_LogonSamLogon(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonSamLogon *r);
void ndr_print_netr_LogonSamLogoff(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonSamLogoff *r);
enum ndr_err_code ndr_push_netr_ServerReqChallenge(struct ndr_push *ndr, int flags, const struct netr_ServerReqChallenge *r);
enum ndr_err_code ndr_pull_netr_ServerReqChallenge(struct ndr_pull *ndr, int flags, struct netr_ServerReqChallenge *r);
void ndr_print_netr_ServerReqChallenge(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerReqChallenge *r);
void ndr_print_netr_ServerAuthenticate(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerAuthenticate *r);
void ndr_print_netr_ServerPasswordSet(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerPasswordSet *r);
void ndr_print_netr_DatabaseDeltas(struct ndr_print *ndr, const char *name, int flags, const struct netr_DatabaseDeltas *r);
void ndr_print_netr_DatabaseSync(struct ndr_print *ndr, const char *name, int flags, const struct netr_DatabaseSync *r);
void ndr_print_netr_AccountDeltas(struct ndr_print *ndr, const char *name, int flags, const struct netr_AccountDeltas *r);
void ndr_print_netr_AccountSync(struct ndr_print *ndr, const char *name, int flags, const struct netr_AccountSync *r);
void ndr_print_netr_GetDcName(struct ndr_print *ndr, const char *name, int flags, const struct netr_GetDcName *r);
void ndr_print_netr_LogonControl(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonControl *r);
void ndr_print_netr_GetAnyDCName(struct ndr_print *ndr, const char *name, int flags, const struct netr_GetAnyDCName *r);
void ndr_print_netr_LogonControl2(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonControl2 *r);
void ndr_print_netr_ServerAuthenticate2(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerAuthenticate2 *r);
void ndr_print_netr_DatabaseSync2(struct ndr_print *ndr, const char *name, int flags, const struct netr_DatabaseSync2 *r);
void ndr_print_netr_DatabaseRedo(struct ndr_print *ndr, const char *name, int flags, const struct netr_DatabaseRedo *r);
void ndr_print_netr_LogonControl2Ex(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonControl2Ex *r);
void ndr_print_netr_NetrEnumerateTrustedDomains(struct ndr_print *ndr, const char *name, int flags, const struct netr_NetrEnumerateTrustedDomains *r);
void ndr_print_netr_DsRGetDCName(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRGetDCName *r);
void ndr_print_netr_LogonGetCapabilities(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonGetCapabilities *r);
void ndr_print_netr_NETRLOGONSETSERVICEBITS(struct ndr_print *ndr, const char *name, int flags, const struct netr_NETRLOGONSETSERVICEBITS *r);
void ndr_print_netr_LogonGetTrustRid(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonGetTrustRid *r);
void ndr_print_netr_NETRLOGONCOMPUTESERVERDIGEST(struct ndr_print *ndr, const char *name, int flags, const struct netr_NETRLOGONCOMPUTESERVERDIGEST *r);
void ndr_print_netr_NETRLOGONCOMPUTECLIENTDIGEST(struct ndr_print *ndr, const char *name, int flags, const struct netr_NETRLOGONCOMPUTECLIENTDIGEST *r);
enum ndr_err_code ndr_push_netr_ServerAuthenticate3(struct ndr_push *ndr, int flags, const struct netr_ServerAuthenticate3 *r);
enum ndr_err_code ndr_pull_netr_ServerAuthenticate3(struct ndr_pull *ndr, int flags, struct netr_ServerAuthenticate3 *r);
void ndr_print_netr_ServerAuthenticate3(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerAuthenticate3 *r);
void ndr_print_netr_DsRGetDCNameEx(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRGetDCNameEx *r);
void ndr_print_netr_DsRGetSiteName(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRGetSiteName *r);
void ndr_print_netr_LogonGetDomainInfo(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonGetDomainInfo *r);
void ndr_print_netr_ServerPasswordSet2(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerPasswordSet2 *r);
void ndr_print_netr_ServerPasswordGet(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerPasswordGet *r);
void ndr_print_netr_NETRLOGONSENDTOSAM(struct ndr_print *ndr, const char *name, int flags, const struct netr_NETRLOGONSENDTOSAM *r);
void ndr_print_netr_DsRAddressToSitenamesW(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRAddressToSitenamesW *r);
void ndr_print_netr_DsRGetDCNameEx2(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRGetDCNameEx2 *r);
void ndr_print_netr_NETRLOGONGETTIMESERVICEPARENTDOMAIN(struct ndr_print *ndr, const char *name, int flags, const struct netr_NETRLOGONGETTIMESERVICEPARENTDOMAIN *r);
void ndr_print_netr_NetrEnumerateTrustedDomainsEx(struct ndr_print *ndr, const char *name, int flags, const struct netr_NetrEnumerateTrustedDomainsEx *r);
void ndr_print_netr_DsRAddressToSitenamesExW(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRAddressToSitenamesExW *r);
void ndr_print_netr_DsrGetDcSiteCoverageW(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsrGetDcSiteCoverageW *r);
void ndr_print_netr_LogonSamLogonEx(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonSamLogonEx *r);
void ndr_print_netr_DsrEnumerateDomainTrusts(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsrEnumerateDomainTrusts *r);
void ndr_print_netr_DsrDeregisterDNSHostRecords(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsrDeregisterDNSHostRecords *r);
void ndr_print_netr_ServerTrustPasswordsGet(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerTrustPasswordsGet *r);
void ndr_print_netr_DsRGetForestTrustInformation(struct ndr_print *ndr, const char *name, int flags, const struct netr_DsRGetForestTrustInformation *r);
void ndr_print_netr_GetForestTrustInformation(struct ndr_print *ndr, const char *name, int flags, const struct netr_GetForestTrustInformation *r);
void ndr_print_netr_LogonSamLogonWithFlags(struct ndr_print *ndr, const char *name, int flags, const struct netr_LogonSamLogonWithFlags *r);
void ndr_print_netr_ServerGetTrustInfo(struct ndr_print *ndr, const char *name, int flags, const struct netr_ServerGetTrustInfo *r);
#endif /* _HEADER_NDR_netlogon */
