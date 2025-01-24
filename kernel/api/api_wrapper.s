extern api
global api_wrapper

api_wrapper:
    push eax
    push ebx
    call api
    iret