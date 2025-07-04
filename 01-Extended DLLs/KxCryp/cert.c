#include "buildcfg.h"
#include "kxcrypp.h"

KXCRYPAPI BOOL WINAPI CertGetCertificateChain_Ext(
    IN HCERTCHAINENGINE hChainEngine,
    IN PCCERT_CONTEXT pCertContext,
    IN LPFILETIME pTime,
    IN HCERTSTORE hAdditionalStore,
    IN PCERT_CHAIN_PARA pChainPara,
    IN DWORD dwFlags,
    IN LPVOID pvReserved,
    OUT PCCERT_CHAIN_CONTEXT *ppChainContext)
{
    // Get the actual certificate
    BOOL result = CertGetCertificateChain(
        hChainEngine, 
        pCertContext, 
        pTime,
        hAdditionalStore, 
        pChainPara,
        dwFlags, 
        pvReserved, 
        ppChainContext
    );

    if (result && ppChainContext && *ppChainContext) {
        // Remove const qualifier to modify the structure
        CERT_CHAIN_CONTEXT* pMutableContext = (CERT_CHAIN_CONTEXT*)*ppChainContext;

        // Apparently just removing the errors works fine? Why, Windows?
        pMutableContext->TrustStatus.dwErrorStatus = 0;
        pMutableContext->TrustStatus.dwInfoStatus |= 
            CERT_TRUST_HAS_EXACT_MATCH_ISSUER |
            CERT_TRUST_HAS_KEY_MATCH_ISSUER |
            CERT_TRUST_HAS_NAME_MATCH_ISSUER |
            CERT_TRUST_IS_SELF_SIGNED;
    }

    return result;
}

KXCRYPAPI BOOL WINAPI CertVerifyCertificateChainPolicy_Ext(
    IN LPCSTR pszPolicyOID,
    IN PCCERT_CHAIN_CONTEXT pChainContext,
    IN PCERT_CHAIN_POLICY_PARA pPolicyPara,
    IN OUT PCERT_CHAIN_POLICY_STATUS pPolicyStatus
) {
    if (!pPolicyStatus)
        return FALSE;

    pPolicyStatus->dwError = 0;
    // set dwChainIndex or dwElementIndex = -1 to indicate "not applicable" 
    pPolicyStatus->lChainIndex   = -1;
    pPolicyStatus->lElementIndex = -1;

    return TRUE;
}