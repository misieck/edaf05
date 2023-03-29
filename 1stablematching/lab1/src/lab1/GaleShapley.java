package lab1;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.Scanner;

public class GaleShapley {

	private static int nbrOfPairs;
	private static int[][] personer;
	private static int[][] företag;

	public static void main(String[] args) {

		inputParser();
		// System.out.println(Arrays.deepToString(personer));
		// System.out.println(Arrays.deepToString(företag));
		galeShapleyPairing();

		return;
	}

	private static void inputParser() {
		Scanner scan = new Scanner(System.in);
		nbrOfPairs = scan.nextInt(); // Sparar antalet personer och företag

		// Skapa en array för alla personer och deras önskelista
		personer = new int[nbrOfPairs][nbrOfPairs];
		företag = new int[nbrOfPairs][nbrOfPairs];

		HashSet<Integer> passedPersons = new HashSet<Integer>();
		// Läs in datan så länge det finns
		while (scan.hasNextInt()) {
			int individNbr = scan.nextInt();
			boolean hasBeenPassed = !passedPersons.add(individNbr);

			for (int i = 0; i < nbrOfPairs; i++) {
				if (hasBeenPassed) {// Lägger in för person individNbr
					personer[individNbr - 1][i] = scan.nextInt() - 1;
				} else {// Lägger in för företag individNbr
					företag[individNbr - 1][i] = scan.nextInt() - 1;
				}
			}

			// System.out.println(hasBeenPassed);

		}

		scan.close();
		return;
	}

	private static void galeShapleyPairing() {
		LinkedList<Integer> alonePersons = new LinkedList<Integer>();
		int[] pairing = new int[nbrOfPairs]; // Företag index
		boolean[][] applied = new boolean[nbrOfPairs][nbrOfPairs];
		for (int i = 0; i < nbrOfPairs; i++) {// Alla personer i en lista
			alonePersons.add(i);
			pairing[i] = -1;
		}
		// System.out.println(Arrays.deepToString(pairing));
		while (!alonePersons.isEmpty()) {
			int currentPerson = alonePersons.pop();

			for (int i = 0; i < nbrOfPairs; i++) {
				int companyPrioI = personer[currentPerson][i];
				boolean current_applied = applied[currentPerson][companyPrioI];
				if (current_applied) {
					continue;
				}

				if (pairing[companyPrioI] == -1) {
					addPairing(pairing, companyPrioI , currentPerson);
					applied[currentPerson][companyPrioI] = true;
					break;
				} else if (companyPrefersAOverB(companyPrioI, currentPerson, pairing[companyPrioI])) {
					int previousPerson = replacePairing(pairing, companyPrioI, currentPerson);
					alonePersons.add(previousPerson);
				} else {
					alonePersons.add(currentPerson);
				}
			}

		}
		for (int i = 0; i < nbrOfPairs; i++) {
			System.out.println(pairing[i] + 1);
		}
		return;
	}

	private static void addPairing(int[] pairing, int company, int person) {
		assert pairing[company] == -1;
		pairing[company] = person;
		return;
	}

	private static int replacePairing(int[] pairing, int company, int person) {
		int ret = pairing[company];
		pairing[company] = person;
		return ret;
	}

	private static boolean companyPrefersAOverB(int company, int personA, int personB) {

		for (int i = 0; i < företag[company].length; i++) {

			if (personA == företag[company][i] || personB == företag[company][i]) {
				return personA == företag[company][i];
			}
		}
		assert false;
		return false;
	}
}
