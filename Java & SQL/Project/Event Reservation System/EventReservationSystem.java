import java.sql.*;
import java.util.Arrays;
import java.util.Scanner;


public class EventReservationSystem {

	static final String DB_URL = "jdbc:mysql://localhost:3306/CS202_1";
	static final String USERNAME = "root";
	static final String PASSWORD = "Mb2709+*!";
	static Connection conn;
	static Statement stmt;
	static Scanner in;
	static ResultSet rs;
	static String sqlCheck;
	static String sqlInsert;
	static String sqlDelete;
	static String o_name;
	static String l_name;
	static String et_id;
	static boolean check;
	static String o_id;
	static String l_id;
	static String r_id;
	static String day_n;
	static String start_h;
	static String end_h;
	static String[] et_id_array;
	static boolean isTrue;
	
	
	public static void main(String[] args) throws SQLException{
		try {
		System.out.println("Establishing connection to " + DB_URL);
		Class.forName("com.mysql.cj.jdbc.Driver");
	    conn = DriverManager.getConnection(DB_URL, USERNAME, PASSWORD);
		} catch (Exception e) {
		e.printStackTrace();
		System.out.println("Database Access Error\n" + "System quitting...");
		System.exit(0);
		}
		System.out.println("Connection established as user " + USERNAME + "\n");
		in = new Scanner(System.in);
		optionsMenu();
	}
	
	public static void optionsMenu() throws SQLException{
		
		System.out.println("OPTIONS MENU\n\n"
				+ "1. Add an Organizer\n"
				+ "2. Remove an Organizer\n"
				+ "3. Add a Location\n"
				+ "4. Remove a Location\n"
				+ "5. Make a Reservation\n"
				+ "6. Cancel a Reservation\n"
				+ "7. List All Reservations\n"
				+ "8. List All Reservations of a Location\n"
				+ "9. List All Reservations of an Organizer\n"
				+ "10. Show Organizer of a Particular Reservation\n"
				+ "11. List All Reservations of a Particular Event Type(Concert, Meeting or Sports Organization)\n"
				+ "12. Exit\n");
		 
		System.out.print("Choose an option from the menu: ");
		int option = 12;
		try {
			check = true;
			option = Integer.parseInt(in.nextLine());
        } catch (Exception e) {
            check = false;
        }
		
		if(check) {
			
			System.out.println("\nOption " + option + " is chosen\n");
			stmt = conn.createStatement();
			
			if(option > 0 && option < 13) {
				
				if(option == 1) {
					addOrganizer();
				}else if(option == 2) {
					deleteOrganizer();
				}else if(option == 3) {
					addLocation();
				}else if(option == 4) {
					deleteLocation();
				}else if(option == 5) {
					makeReservation();
				}else if(option == 6) {
					cancelReservation();
				}else if(option == 7) {
					listReservations(1);
				}else if(option == 8) {
					listReservations(2);
				}else if(option == 9) {
					listReservations(3);
				}else if(option == 10) {
					showOrganizer();
				}else if(option == 11) {
					listReservations(4);
				}else if(option == 12) {
					if (conn != null) {
						conn.close();
					}
					System.out.println("Connection closed, system quitting...");
					System.exit(0);	
				}
				optionsMenu();
			}else{
				System.out.println("\nEnter a number in between 1 and 12\n" +
						"System quitting...");
				System.exit(0);	
			}
		}else {
			System.out.println("\nPlease enter a valid input\n");
			optionsMenu();
		}
		
	}
	
	public static void addOrganizer() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listOrganizer();
			System.out.print("Enter an organizer ID to add an organizer: ");
			o_id  = in.nextLine();
			if(IDCheck(o_id,1) && existsInDB(1,false,o_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			System.out.print("Enter a name to add an organizer: ");
			o_name  = in.nextLine();
			if(nameCheck(o_name,1)) {
				sqlInsert = "INSERT INTO Organizer VALUES('" + o_id + "','" + o_name + "')";
				stmt.executeUpdate(sqlInsert);
				System.out.println("Organizer " + o_name + " added");
				listOrganizer();
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
	}
	
	public static void deleteOrganizer() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listOrganizer();
			System.out.print("Enter an organizer ID to delete an organizer: ");
			o_id  = in.nextLine();
			if(IDCheck(o_id,1) && existsInDB(1,true,o_id)) {
				sqlDelete = "DELETE FROM Organizer WHERE o_id = '" + o_id + "'";
				stmt.executeUpdate(sqlDelete);
				System.out.println("Organizer " + o_id + " deleted\n");
				listOrganizer();
				isTrue = false;	
			}else {
				isTrue = true;
			}
		}
	}
	
	public static void listOrganizer() throws SQLException {
		System.out.println("Organizer list is as follows:\n");
		sqlCheck = "SELECT * FROM Organizer";
		rs = stmt.executeQuery(sqlCheck);
			if(rs.next() == false) {
				System.out.println("EMPTY!\n");
			}else{
				rs.absolute(0);
				while (rs.next()) {
					System.out.println("Organizer ID: " + rs.getString("o_id") + 
							"\nOrganizer Name: " + rs.getString("o_name") + "\n");
				}
			}
	}
	
	public static void addLocation() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listLocation();
			System.out.print("Enter a location ID to add a location: ");
			l_id  = in.nextLine();
			if(IDCheck(l_id,2) && existsInDB(2,false,l_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			System.out.print("Enter a name to add an location: ");
			l_name  = in.nextLine();
			if(nameCheck(l_name,2)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			listHas();
			System.out.print("Enter event type ID(s) for the location (separate with commas): ");
			et_id = in.nextLine();
			et_id_array = et_id.split(",");
			if(IDCheck(et_id,4)) {
				sqlInsert = "INSERT INTO Location VALUES('" + l_id + "','" + l_name + "')";
				stmt.executeUpdate(sqlInsert);
				for(int i = 0; i < et_id_array.length; i++){
					et_id = et_id_array[i];
					sqlInsert = "INSERT INTO Has VALUES('"  + l_id + "', '" + et_id + "')";
					stmt.executeUpdate(sqlInsert);
				}				
				System.out.println("Location " + l_name + " added\n");
				listLocation();
				listHas();
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
	}
	
	public static void deleteLocation() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listLocation();
			listHas();
			System.out.print("Enter a location ID to delete a location: ");
			l_id  = in.nextLine();
			if(IDCheck(l_id,2) && existsInDB(2,true,l_id)) {
				sqlDelete = "DELETE FROM Location WHERE l_id = '" + l_id + "'";
				stmt.executeUpdate(sqlDelete);
				System.out.println("Location " + l_id + " deleted\n");
				listLocation();
				listHas();
				isTrue = false;
			}else {
				isTrue = true;
			}
		}					
	}
	
	public static void listLocation() throws SQLException {
		System.out.println("\nLocation list is as follows:\n");
		sqlCheck = "SELECT * FROM Location";
		rs = stmt.executeQuery(sqlCheck);
		if(rs.next() == false) {
			System.out.println("EMPTY!\n");
		}else{
			rs.absolute(0);
			while (rs.next()) {
				System.out.println("Location ID: " + rs.getString("l_id") + 
						"\nLocation Name: " + rs.getString("l_name") + "\n");
			}
		}
	}
	
	public static void listLocationforET_ID(String et) throws SQLException {
		System.out.println("\nLocation list for the specified event type is as follows:\n");
		sqlCheck = "SELECT * FROM Location WHERE l_id IN(SELECT l_id FROM Has WHERE et_id = '" + et + "')";
		rs = stmt.executeQuery(sqlCheck);
		if(rs.next() == false) {
			System.out.println("EMPTY!\n");
		}else{
			rs.absolute(0);
			while (rs.next()) {
				System.out.println("Location ID: " + rs.getString("l_id") + 
						"\nLocation Name: " + rs.getString("l_name") + "\n");
			}
		}
	}
	
	public static void listHas() throws SQLException {
		System.out.println("\nMeeting-Event Type ID:001\nSports Organization-Event Type ID:002\n"
				+ "Concert-Event Type ID:003\n");
		System.out.println("Location-Event Type pair list is as follows:\n");
		sqlCheck = "SELECT * FROM Has";
		rs = stmt.executeQuery(sqlCheck);
		if(rs.next() == false) {
			System.out.println("EMPTY!\n");
		}else{
			rs.absolute(0);
			while (rs.next()) {
				System.out.println("Location ID: " + rs.getString("l_id") + "\nEvent Type ID: " 
			+ rs.getString("et_id") + "\n");
			}
		}
	}
	
	public static void makeReservation() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listReservations(5);
			System.out.print("Enter a reservation ID to make a reservation: ");
			r_id  = in.nextLine();
			if(IDCheck(r_id,3) && existsInDB(3,false,r_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			listOrganizer();
			System.out.print("Enter an organizer ID to make a reservation: ");
			o_id  = in.nextLine();
			if(IDCheck(o_id,1) && existsInDB(1,true,o_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			System.out.println("\nMeeting-Event Type ID:001\nSports Organization-Event Type ID:002\n"
					+ "Concert-Event Type ID:003\n");
			System.out.print("Enter an event type ID to make a reservation: ");
			et_id  = in.nextLine();
			if(IDCheck(et_id,4) && isEtFit(l_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			listLocationforET_ID(et_id);
			System.out.print("Enter a location ID to make a reservation: ");
			l_id  = in.nextLine();
			if(IDCheck(l_id,2) && existsInDB(2,true,l_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			listReservations(1);
			System.out.print("Enter a day of the month: ");
			day_n  = in.nextLine();
			if(timeCheck(day_n,"0","0",1,l_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
		isTrue = true;
		while(isTrue) {
			System.out.print("Enter a start hour: ");
			start_h = in.nextLine();
			if(timeCheck(day_n,start_h,"0",2,l_id)) {
				isTrue = false;
			}else {
				isTrue = true;
			}
		}	
		isTrue = true;
		while(isTrue) {
			System.out.print("Enter an end hour: ");
			end_h  = in.nextLine();
			if(timeCheck(day_n,start_h,end_h,3,l_id)) {
				sqlInsert = "INSERT INTO Reservations VALUES('" + r_id + "','" + o_id + "','" 
						+ l_id + "','" + et_id + "','" + day_n + "','" + start_h 
						+ "','" + end_h + "')";
				stmt.executeUpdate(sqlInsert);
				System.out.println("Reservation successful!\n");
				listReservations(1);
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
	}
										
	public static void cancelReservation() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listReservations(1);
			System.out.print("Enter an ID of a reservation to cancel a reservation: ");
			r_id  = in.nextLine();
			if(IDCheck(r_id,3) && existsInDB(3,true,r_id)) {
				sqlDelete = "DELETE FROM Reservations WHERE r_id = '" + r_id + "'";
				stmt.executeUpdate(sqlDelete);
				System.out.println("Reservation " + r_id + " canceled\n");
				listReservations(1);
				isTrue = false;
			}else {
				isTrue = true;
			}
		}
	}	
	
	public static void listReservations(int feature) throws SQLException {
		
		if(feature == 1) {
			System.out.println("Reservation list is as follows:\n");
			sqlCheck = "SELECT * FROM Reservations";
		}else if(feature == 2) {
			isTrue = true;
			while(isTrue) {
				listLocation();
				System.out.print("Enter an ID of a location to list its reservations: ");
				l_id = in.nextLine();
				if(IDCheck(l_id,2) && existsInDB(2,true,l_id)) {
					sqlCheck = "SELECT * FROM Reservations WHERE l_id = '" + l_id + "'";
					System.out.println("Reservation list for the specified location ID is as follows:\n");
					isTrue = false;
				}else {
					isTrue = true;
				}
			}
		}else if(feature == 3) {
			isTrue = true;
			while(isTrue) {
				listOrganizer();
				System.out.print("Enter an ID of an organizer to list its reservations: ");
				o_id = in.nextLine();
				if(IDCheck(o_id,1) && existsInDB(1,true,o_id)) {
					sqlCheck = "SELECT * FROM Reservations WHERE o_id = '" + o_id + "'";
					System.out.println("Reservation list for the specified organizer ID is as follows:\n");
					isTrue = false;
				}else {
					isTrue = true;
				}
			}
		}else if(feature == 4){
			isTrue = true;
			while(isTrue) {
				System.out.println("\nMeeting-Event Type ID:001\nSports Organization-Event Type ID:002\n"
						+ "Concert-Event Type ID:003\n");
				System.out.println("Enter an ID of an event type to list all its related reservations: ");
				et_id = in.nextLine();
				if(IDCheck(et_id,4)) {
					sqlCheck = "SELECT * FROM Reservations WHERE et_id = '" + et_id+ "'";
					System.out.println("Reservation list for the specified event type ID is as follows:\n");
					isTrue = false;
				}else {
					isTrue = true;
				}
			}
		}else {
			sqlCheck = "SELECT r_id FROM Reservations";
			System.out.println("Reservation IDs present in reservation list is as follows:\n");
		}
		rs = stmt.executeQuery(sqlCheck);
		if(rs.next() == false) {
			if(feature == 1 || feature == 5) {
				System.out.println("EMPTY!\n");
			}else if(feature == 2) {
				System.out.println("\nReservation for the given location name doesn't exist\n");
			}else if(feature == 3) {
				System.out.println("\nReservation for the given organizer name doesn't exist\n");
			}else if(feature == 4) {
				System.out.println("\nReservation for the particular event type doesn't exist\n");
			}
		}else{
			if(feature == 5) {
				rs.absolute(0);
				while (rs.next()) {
					System.out.println("Reservation ID: " + rs.getString("r_id") + "\n");
				}
			}else {
				printReservations(rs);
			}
		}
	}
	
	public static void printReservations(ResultSet rs) throws SQLException {
		rs.absolute(0);
		while (rs.next()) {
			System.out.println("Reservation ID: " + rs.getString("r_id") +
			"\nOrganizer ID: " + rs.getString("o_id") +
			"\nLocation ID: " + rs.getString("l_id") +
			"\nEvent Type ID: " + rs.getString("et_id") +
			"\nDay of the month: " + rs.getInt("day_n") +
			"\nStart hour: " + rs.getInt("start_h") + 
			"\nEnd hour: " + rs.getInt("end_h") + "\n");
		}
	}
	
	public static boolean IDCheck(String id, int IDNo) throws SQLException {
		boolean test = true;
		
			if(IDNo == 4) {
				String[] event_types = id.split(",");
				for(int i = 0; i < event_types.length; i++) {
					if(event_types[i].length() != 3 || event_types[i].contains("'") || event_types[i].contains("\"")) {
						System.out.println("\nEvent Type ID should be 3 characters"
								+ "\nand shouldn't contain ' and \"\n");
						test = false;
					}else {
						boolean look = Arrays.stream(event_types).anyMatch("001"::equals) ||
								Arrays.stream(event_types).anyMatch("002"::equals) ||
								Arrays.stream(event_types).anyMatch("003"::equals);
						
						if(!look) {
							System.out.println("\nEvent Type ID should be 001, 002, or 003\n");
						}
						return look;
					}
				}
			}else {	
				if(id.length() != 3 || id.contains("'") || id.contains("\"")) {
					if(IDNo == 1) {
						System.out.println("\nOrganizer ID should be 3 characters"
								+ "\nand shouldn't contain ' and \"\n");
					}else if(IDNo == 2) {
						System.out.println("\nLocation ID should be 3 characters"
								+ "\nand shouldn't contain ' and \"\n");
					}else if(IDNo == 3) {
						System.out.println("\nReservation ID should be 3 characters"
								+ "\nand shouldn't contain ' and \"\n");
					}
					test = false;	
				}
			}
			return test;
		}
	
	public static boolean nameCheck(String name, int nameNo) throws SQLException {
		boolean test = true;
		
			if((name.length() > 30 || name.contains("'") || name.contains("\"") || name.length() == 0)) {
				if(nameNo == 1) {
					System.out.println("\nOrganizer name can't be empty, more than 30 characters"
							+ "\nand shouldn't contain ' and \"\n");
				}else if(nameNo == 2) {
					System.out.println("\nLocation name can't be empty, more than 30 characters"
							+ "\nand shouldn't contain ' and \"\n");
				}
				test = false;
			}else {
				test = true;
			}
			return test;	
		}
		
	public static boolean timeCheck(String day_n,  String start_h, String end_h, int timeID, String l_id) throws SQLException{
		
		boolean test = false;
	
		try {
			int day_n_Int = Integer.parseInt(day_n);
			int start_h_Int = Integer.parseInt(start_h);
			int end_h_Int = Integer.parseInt(end_h);
			test = true;
			if(test) {
				if(timeID == 1) {
					 if(!(day_n_Int > 0 && day_n_Int < 32 && day_n.length() == 2)) {
						 System.out.println("\nDay of the month should be 2 characters"
									+ "\nand should be in between 1 and 31\n");
						 test = false;
					 }
				}else if(timeID == 2) {
					if(start_h_Int > 0 && start_h_Int < 25 && start_h.length() == 2) {
						sqlCheck = "SELECT COUNT(*) FROM Reservations WHERE "
								+ "day_n = '" + day_n_Int + "'" + " AND l_id = '" + l_id + "'";
						rs = stmt.executeQuery(sqlCheck);
						rs.absolute(0);
						if(rs.next() == true) {
							sqlCheck = "SELECT start_h,end_h FROM Reservations WHERE "
									+ "day_n = '" + day_n_Int + "'" + " AND l_id = '" + l_id + "'";
							rs = stmt.executeQuery(sqlCheck);
							rs.absolute(0);
							while(rs.next()) {
									if(start_h_Int > Integer.parseInt(rs.getString("end_h"))) {
										test = true;
									}else if(start_h_Int < Integer.parseInt(rs.getString("start_h"))) {
										test = true;
									}else {
										test = false;
										break;
									}
							}
							if(!test) {
								System.out.println("\nThere exists a reservation at that time "
										+ "for the specified location\n");
							}
						}
					}else {
						 System.out.println("\nStart hour should be 2 characters"
									+ "\nand should be in between 1 and 24\n");
						 test = false;
					 }
				}else {
					if(end_h_Int > 0 && end_h_Int < 25 && end_h.length() == 2 && end_h_Int > start_h_Int) {
						sqlCheck = "SELECT start_h,end_h FROM Reservations WHERE "
								+ "day_n = '" + day_n_Int + "'" + " AND l_id = '" + l_id + "'";
						rs = stmt.executeQuery(sqlCheck);
						rs.absolute(0);
						while(rs.next()) {
								if(start_h_Int > Integer.parseInt(rs.getString("end_h"))) {
									test = true;
								}else if(start_h_Int < Integer.parseInt(rs.getString("start_h")) 
										&& end_h_Int < Integer.parseInt(rs.getString("start_h"))) {
									test = true;
								}else {
									test = false;
									break;
								}
						}
						if(!test) {
							System.out.println("\nThere exists a reservation at that time "
									+ "for the specified location\n");
						}
					 }else {
						 System.out.println("\nEnd hour should be 2 characters, in between 1 and 24"
						 		+ "\nand bigger than start hour\n");
						 test = false;
					 }
				}
			}
		}catch(NumberFormatException e){
			System.out.println("\nPlease enter a valid input\n");
			return false;
		}
		return test;	
	}
	
	public static boolean existsInDB(int IDNo, boolean yesORno, String textInput) throws SQLException {
		boolean test = false;
		if(IDNo == 1) {
			sqlCheck = "SELECT COUNT(*) FROM Organizer WHERE o_id = '" + textInput + "'";
			rs = stmt.executeQuery(sqlCheck);
			rs.absolute(1);
			if(yesORno) {
				if(rs.getInt(1) == 1) {
					test = true;
				}else {
					System.out.println("\nOrganizer with the specified ID doesn't exist\n");
				}
			}else {
				if(rs.getInt(1) == 0) {
					test = true;
				}else {
					System.out.println("\nOrganizer with the specified ID already exists\n");
				}
			}
		}else if(IDNo == 2){
			sqlCheck = "SELECT COUNT(*) FROM Location WHERE l_id = '" + textInput + "'";
			rs = stmt.executeQuery(sqlCheck);
			rs.absolute(1);
			if(yesORno) {
				if(rs.getInt(1) == 1) {
					test = true;
				}else {
					System.out.println("\nLocation with the specified ID doesn't exist\n");
				}
			}else {
				if(rs.getInt(1) == 0) {
					test = true;
				}else {
					System.out.println("\nLocation with the specified ID already exists\n");
				}
			}
		}else {
			sqlCheck = "SELECT COUNT(*) FROM Reservations WHERE r_id = '" + textInput + "'";
			rs = stmt.executeQuery(sqlCheck);
			rs.absolute(1);
			if(yesORno) {
				if(rs.getInt(1) == 1) {
					test = true;
				}else {
					System.out.println("\nReservation with the specified ID doesn't exist\n");
				}
			}else {
				if(rs.getInt(1) == 0) {
					test = true;
				}else {
					System.out.println("\nReservation with the specified ID already exists\n");
				}
			}
		}
		return test;
	}
	
	public static boolean isEtFit(String l_id) throws SQLException {
		sqlCheck = "SELECT et_id FROM Has WHERE l_id = '" + l_id + "'"; 
		rs = stmt.executeQuery(sqlCheck);
		rs.absolute(0);
		while(rs.next()) {
			if(et_id.equals(rs.getString("et_id"))){
				check = true;
				break;
			}else {
				check = false;
			}
		}
		if(check) {
			return check;
		}else {
			System.out.println("\nSpecified event type ID doesn't match\n");
			return check;
		}
	}
	
	public static void showOrganizer() throws SQLException {
		isTrue = true;
		while(isTrue) {
			listReservations(5);
			System.out.print("Enter an ID of a reservation to show its organizer: ");
			r_id  = in.nextLine();
			if(IDCheck(r_id,3) && existsInDB(3,true,r_id)) {
				sqlCheck = "SELECT o_name FROM Organizer WHERE o_id in "
						+ "(SELECT o_id FROM Reservations WHERE r_id = '" + r_id + "')";
				rs = stmt.executeQuery(sqlCheck);
				rs.absolute(1);
				System.out.println("Organizer name of the specified reservation is " + rs.getString("o_name") + "\n");
				isTrue = false;
			}else {
				isTrue = true;
			}
		}	
	}
	
	}

		
	

	
	

