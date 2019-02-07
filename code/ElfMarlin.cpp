#include <iostream>
#include "ElfMarlin.hpp"

//TODO: сделать нормальный log файл

#define C(V) case PT_##V: s = #V; break
#define PRINT_FMT " %-20s 0x%jx\n "
#define PRINT_FIELD(N) do { \
        printf ( PRINT_FMT , #N , ( uintmax_t ) ehdr.N ); \
        }  while (0)
        
        
void print_ptype(size_t pt)
{
    const char * s;
    switch (pt)
    {
        C(NULL);
        C(LOAD);
        C(DYNAMIC);
        C(INTERP);
        C(NOTE);
        C(SHLIB);
        C(PHDR);
        C(TLS);
        C(SUNWBSS);
        C(SUNWSTACK);
        default :s = " unknown ";
            break;
    }
    printf( " \"%s \" " ,s);

}
#undef C


ElfMarlin::ElfMarlin(const char * path_to_elf, int log)
{
    int class_;
    char * id ;
    GElf_Ehdr ehdr ;
    
    if ( elf_version ( EV_CURRENT ) == EV_NONE )
        errx ( EXIT_FAILURE , " ELF library initialization "
                              " failed : %s " , elf_errmsg ( -1));
    if (( fd = open ( path_to_elf , O_RDONLY , 0)) < 0)
        err ( EXIT_FAILURE , " open \"%s \" failed " , path_to_elf);
    if (( e = elf_begin ( fd , ELF_C_READ , nullptr )) == nullptr )
        errx ( EXIT_FAILURE , " elf_begin () failed : %s . " ,
               elf_errmsg ( -1));
    if ( elf_kind ( e ) != ELF_K_ELF )
        errx ( EXIT_FAILURE , " \"%s \" is not an ELF object . " ,
               path_to_elf);
    if ( gelf_getehdr (e , & ehdr ) == nullptr )
        errx ( EXIT_FAILURE , " getehdr () failed : %s . " ,
           elf_errmsg ( -1));
    if (( class_ = gelf_getclass ( e )) == ELFCLASSNONE )
        errx ( EXIT_FAILURE , " getclass () failed : %s . " ,
           elf_errmsg ( -1));
    
    if (log)
    {
        printf(" %s : %d - bit ELF object \n ", path_to_elf,
               class_ == ELFCLASS32 ? 32 : 64);
    }
    
   
    
    if (( id = elf_getident (e , nullptr )) == nullptr )
        errx ( EXIT_FAILURE , " getident () failed : %s . " ,
           elf_errmsg ( -1));
    if (log)
    {
        printf(" %3s e_ident [0..%1d] %7s ", " ",
               EI_ABIVERSION, " ");
    
        for (int i = 0; i <= EI_ABIVERSION; i++)
        {
            printf(" [ %X ] ", id[i]);
        }
        printf(" \n ");
    
        PRINT_FIELD ( e_type );
        PRINT_FIELD ( e_machine );
        PRINT_FIELD ( e_version );
        PRINT_FIELD ( e_entry );
        PRINT_FIELD ( e_phoff );
        PRINT_FIELD ( e_shoff );
        PRINT_FIELD ( e_flags );
        PRINT_FIELD ( e_ehsize );
        PRINT_FIELD ( e_phentsize );
        PRINT_FIELD ( e_shentsize );
    }
    i_class = class_;
    type = ehdr.e_type;
    machine = ehdr.e_machine;
    i_data = id[5];
    if ( i_class == ELFCLASS32)
    {
        entry32 = static_cast<uint32_t>(ehdr.e_entry);
        entry64 = 0;
    }
    else if (i_class == ELFCLASS64)
    {
        entry64 = ehdr.e_entry;
        entry32 = 0;
    }
    else
    {
        entry64 = 0;
        entry32 = 0;
    }
    
    flags = ehdr.e_flags;
    
    if (!check())
    {
        std::cerr<<"error: elf initialize fail"<<std::endl;
        throw 63;
    }
}


bool  ElfMarlin::check()
{
    int c = i_class * type * machine * i_data;
    if (i_class == ELFCLASS32)
    {
        c *= entry32;
    }
    else if (i_class == ELFCLASS64)
    {
        c *= entry64;
    }
    else
    {
        return false;
    }
    return c != 0;
    
}
void ElfMarlin::load(MMU &mmu)
{
    size_t phdrnum;
    if  (elf_getphdrnum(e, &phdrnum))
    {
        std::cerr<<"error: can't get phdrnum"<<std::endl;
        throw 63;
    }
    std::vector<char> buf(512);
    GElf_Phdr phdr;
    for (int i = 0; static_cast<size_t>(i)< phdrnum; i++ )
    {
        if ( gelf_getphdr(e , i , &phdr) != &phdr )
        {
            std::cerr<<"error: can't get "<<i<<" phdr"<<std::endl;
            throw 63;
        }
        if (phdr.p_type != PT_LOAD)
            continue;
        uint64_t offset = phdr.p_offset;
        uint64_t fsize = phdr.p_filesz;
        uint64_t msize = phdr.p_memsz;
        uint64_t flags = phdr.p_flags;
        uint64_t vaddress = phdr.p_vaddr;
        if (buf.size() < fsize)
        {
            buf.resize(fsize);
        }
        
        if (offset != static_cast<uint64_t >(lseek(fd, offset, SEEK_SET)))
        {
            std::cerr<<"error: lseek failed"<<std::endl;
            throw 63;
        }
        ssize_t read_bytes = read(fd, &buf[0], fsize);
        if (read_bytes <0 || fsize != static_cast<uint32_t>(read_bytes))
        {
            std::cerr<<"error: read instr fail"<<std::endl;
            throw 63;
        }
        
        mmu.upload_segment_to_mem(&buf[0], vaddress,flags,fsize,msize);
        
    }
}

ElfMarlin::~ElfMarlin()
{
    elf_end( e );
    close( fd );
}