/* Ancre la machinerie libc++ dont les filtres C++ ont besoin dans le module
 * principal.
 *
 * Compiles en release, les filtres C++ (libheif, poppler, libraw...) importent
 * via GOT.mem/GOT.func des vtables, typeinfo et destructeurs de libc++, en
 * entrees "required". Quand l'une manque, loadDylibs echoue avec
 * "bad export type for '<symbole>': undefined" - et les revele une par une.
 *
 * Deux conditions sont necessaires, aucune ne suffit seule :
 *   - le symbole doit EXISTER dans le module principal : c'est le role de cette
 *     unite de compilation, qui instancie les types concernes ;
 *   - il doit etre EXPORTE : MAIN_MODULE=2 n'exporte que ce que liste
 *     exports.cmake (groupe CXX_RUNTIME).
 *
 * Ne jamais lister dans exports.cmake un symbole que rien ne definit : il
 * devient une entree required non resolue et casse le chargement de TOUS les
 * filtres. Le lien previent alors par "undefined symbol: <nom>" - c'est le
 * garde-fou a verifier apres toute modification de cette liste.
 *
 * Aucune de ces fonctions n'est appelee ; elles sont exportees pour survivre
 * a l'elimination de code mort. */
#include <sstream>
#include <ostream>
#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <typeinfo>

namespace {
struct AnchorBase { virtual ~AnchorBase() {} virtual void f() {} };
struct AnchorDerived : AnchorBase { void f() override {} };
}

extern "C" void gpac_cxx_stream_anchor(void)
{
    std::ostringstream oss;
    oss << 0;
    std::string s = oss.str();
    std::stringstream ss(s);
    int v = 0;
    ss >> v;
    (void) v;
    std::cerr.flush();
}

extern "C" void gpac_cxx_except_anchor(void)
{
    try { throw std::out_of_range("a"); } catch (const std::exception &) {}
    try { throw std::length_error("a"); } catch (const std::exception &) {}
}

extern "C" void gpac_cxx_rtti_anchor(void)
{
    AnchorDerived d;
    AnchorBase *b = &d;
    AnchorDerived *p = dynamic_cast<AnchorDerived *>(b);
    (void) p;
    std::shared_ptr<AnchorDerived> sp = std::make_shared<AnchorDerived>();
    std::weak_ptr<AnchorDerived> wp = sp;
    (void) wp.lock();
}

/* Statique locale non triviale : force l'emission de __cxa_guard_acquire /
 * _release, l'ABI d'initialisation thread-safe des statiques de fonction.
 * libjxl en utilise ; sans cela son premier appel echoue sur
 * "TypeError: resolved is not a function" - le stub silencieux qu'emscripten
 * cree pour une fonction env non resolue. */
extern "C" int gpac_cxx_guard_anchor(void)
{
    static const std::string once("x");
    return (int) once.size();
}

/* RTTI des enums et de l'heritage virtuel multiple. libraw declenche
 * __enum_type_info ; l'heritage en losange declenche __vmi_class_type_info.
 * Sans eux, loadDylibs echoue sur "bad export type for
 * '_ZTVN10__cxxabiv116__enum_type_infoE'". */
namespace {
enum class AnchorEnum { A, B };
struct VBase { virtual ~VBase() {} virtual void g() {} };
struct VLeft : virtual VBase { void g() override {} };
struct VRight : virtual VBase { void g() override {} };
struct VDiamond : VLeft, VRight { void g() override {} };
}

extern "C" const char *gpac_cxx_enumrtti_anchor(void)
{
    VDiamond d;
    VBase *b = &d;
    VDiamond *p = dynamic_cast<VDiamond *>(b);
    (void) p;
    return typeid(AnchorEnum).name();
}

/* Instanciations supplementaires reclamees par les filtres image apres la
 * remise a niveau des sources : libjxl trie des int* et des size_t*, libheif
 * construit une std::locale par defaut et manipule un basic_streambuf. Sans
 * ces instanciations, exporter les symboles correspondants creerait des
 * entrees GOT non resolues. */
#include <algorithm>
#include <locale>
#include <vector>
#include <cstddef>

extern "C" std::size_t gpac_cxx_sort_anchor(void)
{
    int vi[3] = {3, 1, 2};
    std::size_t vm[3] = {3, 1, 2};
    std::sort(vi, vi + 3);
    std::sort(vm, vm + 3);
    return (std::size_t) vi[0] + vm[0];
}

extern "C" void gpac_cxx_locale_anchor(void)
{
    std::locale loc;
    (void) loc;
}
