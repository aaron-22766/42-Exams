
#include "Dummy.hpp"
#include "Fwoosh.hpp"
#include "Warlock.hpp"

void leaks(void) {
    system("leaks test");
}

int main()
{
    atexit(leaks);
  Warlock richard("Richard", "the Titled");

  Dummy bob;
  Fwoosh* fwoosh = new Fwoosh();
  Fwoosh* fwoosh2 = new Fwoosh();

  richard.learnSpell(fwoosh);
  richard.learnSpell(fwoosh2);

  richard.introduce();
  richard.launchSpell("Fwoosh", bob);

  richard.forgetSpell("Fwoosh");
  richard.launchSpell("Fwoosh", bob);

  delete fwoosh;
  delete fwoosh2;
}
