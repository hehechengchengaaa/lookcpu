#include <stdio.h>
#include "core/allin.h"


typedef enum class doing{
    help,
    bang_zhu,
    all,
    quan_bu,
    cpu,
    gpu,
    xian_ka,
    memory,
    nei_cun,
    block,
    yin_pan,
    board,
    zhu_ban,
    net,
    wang_luo,
    power,
    dian_yuan,
    vital,
    jian_duan,
}doing;

doing getdoing(char *argv);

int main(int argc, char *argv[]) {

    if(argc == 1){
        show_cpu();
        show_gpu();
        show_memory();
        show_mem();
        show_block();
        show_partition();
        show_board();
        show_bios();
        show_net();
        show_power();
        show_os();
        printf("You can enter \"lookcpu help\" to get help\n");
        return 0;
    }



    for(int i = 1; i < argc; i++){

        switch(getdoing(argv[i])){
            case doing::help:
                help();
                break;
            case doing::all:
                show_cpu();
                show_gpu();
                show_memory();
                show_mem();
                show_block();
                show_partition();
                show_board();
                show_bios();
                show_net();
                show_power();
                show_os();
                break;
            case doing::cpu:
                show_cpu();
                break;
            case doing::gpu:
                show_gpu();
                break;
            case doing::memory:
                show_mem();
                show_memory();
                break;
            case doing::block:
                show_block();
                show_partition();
                break;
            case doing::board:
                show_board();
                show_bios();
                break;
            case doing::net:
                show_net();
                break;
            case doing::power:
                show_power();
                break;
            case doing::vital:
                show_cpu();
                show_gpu();
                show_memory();
                show_block();
                show_partition();
                show_board();
                break;
            default:
                printf("error?\n");
                printf("You can get sourse code from \n");
                break;

        }



    }

    return 0;
}




doing getdoing(char *argv){

    
    if(!strcmp(argv, "help")  ||  !strcmp(argv,"bang_zhu")){
        return doing::help;
    }
    if(!strcmp(argv, "all") || !strcmp(argv, "quan_bu")){
        return doing::all;
    }
    if(!strcmp(argv, "cpu")){
        return doing::cpu;
    }
    if(!strcmp(argv, "gpu") || !strcmp(argv, "xian_ka")){
        return doing::gpu;
    }
    if(!strcmp(argv, "memory") || !strcmp(argv, "nei_cun")){
        return doing::memory;
    }
    if(!strcmp(argv, "block") || !strcmp(argv, "yin_pan")){
        return doing::block;
    }
    if(!strcmp(argv, "board") || !strcmp(argv, "zhu_ban")){
        return doing::board;
    }
    if(!strcmp(argv, "net") || !strcmp(argv, "wang_luo")){
        return doing::net;
    }
    if(!strcmp(argv, "power") || !strcmp(argv, "dian_yuan")){
        return doing::power;
    }
    if(!strcmp(argv, "vital") || !strcmp(argv, "jian_duan")){
        return doing::vital;
    }
    
    return doing::help;
}