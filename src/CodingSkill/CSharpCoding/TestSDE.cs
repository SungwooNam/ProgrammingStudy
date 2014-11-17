using System;
using System.Collections.Generic;
using System.Linq;
using NUnit.Framework;


namespace CSharpTest
{
    // there seems to be four structure on this. 
    // first is a set with desirability as key and Space as value. This is to figure out the highest desirability space.
    //  -> say it is Desired
    // second is a set with Space.ID as key and Space as value. This is to figure out what is the available space and what is occupied
    //   -> Say it is Free
    // -> key is to synchronize Desired and Free as these two are sorted in different way and will be updated not exactly same moment. 


    // Taken : where car is parked

    // and there should be a temporary buffer that is to store the suggested space to driver when arrived. This is to store suggested spaces 
    // so that I know which space is not available for suggestion
    //  -> say it is Reserved 

    public class SpaceAssigner : GarageStatusListener
    {
        class TakenSpace
        {
            public Car Car { get; set; }
            public Space Space { get; set; }

            public TakenSpace(Car c, Space s)
            {
                Car = c;
                Space = s;
            }
        }

        DesiredSpaces Desired;
        SortedDictionary<int, Space> Free;
        SortedDictionary<int, TakenSpace> Taken;
        SortedDictionary<string, Space> Reserved;


        class DesiredSpaces
        {
            SortedDictionary<int, Stack<Space>> Spaces; // There can be numer of space with same desirebility

            class InDescendingOrder : IComparer<int>
            {
                public int Compare(int x, int y)
                {
                    return y - x;
                }
            }

            public DesiredSpaces()
            {
                Spaces = new SortedDictionary<int, Stack<Space>>(new InDescendingOrder());
            }

            public void Add(Space space)
            {
                Stack<Space> spaces = null;
                if (Spaces.TryGetValue(space.getDesirability(), out spaces) == false)
                {
                    spaces = new Stack<Space>();
                    Spaces.Add(space.getDesirability(), spaces);
                }
                spaces.Push(space);
            }

            public Space PopSpace(Car car, SortedDictionary<int, Space> free)
            {
                bool found = false;
                Space foundSpace = null;
                foreach (KeyValuePair<int, Stack<Space>> pair in Spaces)
                {
                    Stack<Space> spaces = pair.Value;
                    for (; spaces.Count > 0; )
                    {
                        foundSpace = pair.Value.Pop();
                        if (free.ContainsKey(foundSpace.getID()))
                        {
                            found = true;
                            if (spaces.Count == 0)
                            {
                                Spaces.Remove(pair.Key);    // this should make PopSpace to be O(1)
                            }

                            break;
                        }
                    }

                    if (found)
                        break;
                }

                return foundSpace;
            }
        }


        /**
         * Initiates the SpaceAssigner. This method is called only once per
         * app start-up.
         * @param garage The parking garage for which you are vending spaces.
         *
         *  Time complexity O( logn ), space O(n)
         *
         *  space is O(n) though all 4 strcutures are just recording reference of the Space.
         */
        public void initialize(ParkingGarage garage)
        {
            Desired = new DesiredSpaces();
            Free = new SortedDictionary<int, Space>();
            Taken = new SortedDictionary<int, TakenSpace>();
            Reserved = new SortedDictionary<string, Space>();

            foreach (Space s in garage.getSpaces())
            {
                Free.Add(s.getID(), s);
                Desired.Add(s);
            }

            garage.register(this);
        }

        /**
         * Assigns a space to an incoming car and returns that space.
         * 
         * @param car The incoming car that needs a space.
         * @returns The space reserved for the incoming car.
         *
         * Now it is O(1) for Deired.PopSpace
         * Though Reserved.Add() can be O(logn) at worst case
         */
        public Space assignSpace(Car car)
        {
            // takeout a Space from DesiredSpaces ( Space.Desirebility, [Space] )
            //   -> try until the space is also on FreeSpace.  driver can take any place !
            // 
            // put it to ReservedSpaces ( Car.id, [ Space, Car] )
            Space reserved = Desired.PopSpace(car, Free);
            if (reserved != null)
            {
                Reserved.Add(car.getLicensePlateNumber(), reserved);
            }

            return reserved;
        }

        /**
         * {@inheritDoc}
         *
         * O(logn) for time 
         */
        public void onSpaceTaken(Car car, Space space)
        {
            // put the space to TakenSpaces ( Space.ID, [Car,Space] )
            // remove the space from FreeSpaces ( Space.ID, [Space] )
            // take out Car from ReservedSpaces , ... there is chance that driver can just moving to a new spot.. 
            //   if reserved one, remove from ReservedSpaces
            //   if not reserved one, look for reserved one and put it to the DesiredSpaces.

            Taken.Add(space.getID(), new TakenSpace(car, space));

            if (Free.Remove(space.getID()) == false)
            {
                throw new ArgumentException(string.Format(
                    "The space {0} tried by {1} is already taken",
                    space.getID(),
                    car.getLicensePlateNumber()));
            }

            Space reserved = null;
            if (Reserved.TryGetValue(car.getLicensePlateNumber(), out reserved) == false)
            {
                throw new ArgumentException(string.Format(
                    "This car {0} entered garage without getting space assigned",
                    car.getLicensePlateNumber()));
            }
            Reserved.Remove(car.getLicensePlateNumber());

            if (reserved.getID() != space.getID())
            {
                Desired.Add(reserved);
            }
        }

        /**
         * {@inheritDoc}
         *
         * O(logn) for time
         */
        public void onSpaceFreed(Car car, Space space)
        {
            //  remove the space from the TakenSpaces 
            // add the space to the FreeSpaces ( Space.ID, [Car.Space] )
            // add the space to the DesiredSpaces.

            if (Taken.Remove(space.getID()) == false)
            {
                throw new ArgumentException(string.Format(
                    "The space {0} was free before being freed by {1}",
                    space.getID(),
                    car.getLicensePlateNumber()));
            }

            Free.Add(space.getID(), space);
            Desired.Add(space);
        }

        /**
         * {@inheritDoc}
         * 
         * <<insert runtime and memory analysis here>>
         */
        public void onGarageExit(Car car)
        {
            // reading the requirements, it seems that I have to free Reserved when car exit. 
            // So that the reserved is valid all the time when driver is in garage. Or not ? 
            // Though it will be waste of space, if driver puts his/her care on a non-assgined spots but still keep the reserved one.
            // I say once the car parked in any space, then that is the end of the reservation. 
            // unless you give me interface that the space is taken for some times... 
            // so that we are sure that the driver is sure about his/her place.

            // can't think about any good thing to do at the exit... 
            // it may be a similar reason as you can't throw exception on the class destructor in C++
        }
    }


    /**
     * The main app controlling the parking garage.
     */
    public interface ParkingGarage
    {
        /**
         * Registers the given garage status listener to receive notifications for
         * changes in the occupied status of a space.
         * @param assigner The GarageStatusListener responsible for issuing spaces.
         */
        void register(GarageStatusListener assigner);

        /**
         * @return the list of spaces in the parking garage. Note: This list may be 
         * very large and take a long time to iterate through.
         */
        IEnumerable<Space> getSpaces();
    }

    /**
     * Represents a car trying to park in the parking garage.
     */
    public interface Car
    {
        /**
         * @return The state in which the license plate was issued.
         */
        String getLicensePlateState();

        /**
         * @return The license plate number of the car.
         */
        String getLicensePlateNumber();
    }

    /**
     * Represents a space in the garage in which a car can park.
     */
    public interface Space
    {
        /**
         * @return A unique identifier for the given space.
         */
        int getID();

        /**
         * @return An integer representing the desirability of the space.
         *         Spaces with higher values are considered more desirable.
         */
        int getDesirability();

        /**
         * @return true if the space is currently occupied with a car; 
         *         false, otherwise. This returns the real world state of
         *         the Space.
         */
        bool isOccupied();

        /**
         * @return the Car that is currently occupying the Space or null
         *         if no Car is currently present. This returns the real
         *         world state of the space.
         */
        Car getOccupyingCar();
    }

    /**
     * An interface used to receive callbacks about changes in the status
     * of Spaces and cars in the garage. Implementers will receive notifications
     * whenever a space becomes occupied or unoccupied and whenever a car 
     * leaves the garage.
     */
    public interface GarageStatusListener
    {
        /**
         * Invoked whenever a car parks in a space.
         * @param car The car parking in the space.
         * @param space The space being occupied.
         */
        void onSpaceTaken(Car car, Space space);

        /**
         * Invoked whenever a car leaves a space.
         * @param car The car leaving the space.
         * @param space The space that the car left.
         */
        void onSpaceFreed(Car car, Space space);

        /**
         * Invoked whenever a car leaves the garage.
         * @param car The car leaving the garage.
         */
        void onGarageExit(Car car);
    }

    public class TestParkingLot : Space
    {
        public TestParkingLot(int id, int d, bool o, Car car)
        {
            ID = id;
            Desirability = d;
            IsOccupied = o;
            OccupyingCar = car;
        }
        
        int ID;
        int Desirability;
        bool IsOccupied;
        Car OccupyingCar;

        public int getID()
        {
            return ID;
        }

        public int getDesirability()
        {
            return Desirability;
        }

        public bool isOccupied()
        {
            return IsOccupied;
        }

        public Car getOccupyingCar()
        {
            return OccupyingCar;
        }
    }

    public class TestParkingCar : Car
    {
        string PlateState;
        string PlateNumber;

        public TestParkingCar(string number, string state)
        {
            PlateNumber = number;
            PlateState = state;
        }

        public String getLicensePlateState()
        {
            return PlateState;
        }

        public String getLicensePlateNumber()
        {
            return PlateNumber;
        }
    }

    public class TestGarage : ParkingGarage
    {
        IList<Space> Spaces = new List<Space>();

        public void Add(Space s)
        {
            Spaces.Add(s);
        }

        public void register(GarageStatusListener assigner)
        {
        }

        public IEnumerable<Space> getSpaces()
        {
            return Spaces;
        }
    }

    [TestFixture]
    public class UnitTestGarage
    {

        [Test]
        public void TestNormalFlow()
        {
            TestParkingLot[] parkingLots = { 
                new TestParkingLot(0, 100, false, null),
                new TestParkingLot(1, 90, false, null),
                new TestParkingLot(2, 120, false, null),
                new TestParkingLot(3, 100, false, null) };

            TestGarage garage = new TestGarage();
            foreach( Space s in parkingLots )
            {
                garage.Add( s );
            }

            SpaceAssigner assigner = new SpaceAssigner();
            assigner.initialize(garage);

            Car AAA = new TestParkingCar("AAA", "");
            Space s0 = assigner.assignSpace( AAA );
            Assert.That( s0.getDesirability(), Is.EqualTo( 120 ) );
            assigner.onSpaceTaken(AAA, s0);
            assigner.onSpaceFreed(AAA, s0);
            assigner.onGarageExit(AAA);

            s0 = assigner.assignSpace(AAA);
            Assert.That(s0.getDesirability(), Is.EqualTo(120));
            assigner.onSpaceTaken(AAA, s0);
            assigner.onSpaceFreed(AAA, s0);
            assigner.onGarageExit(AAA);
        }

        [Test]
        public void TestDriverTakingNonReservedSpace()
        {
            TestParkingLot[] parkingLots = { 
                new TestParkingLot(0, 100, false, null),
                new TestParkingLot(1, 90, false, null),
                new TestParkingLot(2, 120, false, null),
                new TestParkingLot(3, 100, false, null) };

            TestGarage garage = new TestGarage();
            foreach (Space s in parkingLots)
            {
                garage.Add(s);
            }

            SpaceAssigner assigner = new SpaceAssigner();
            assigner.initialize(garage);

            Car C1 = new TestParkingCar("C1", "");
            Space s1 = assigner.assignSpace(C1);
            assigner.onSpaceTaken(C1, parkingLots[0]);

            Car C2 = new TestParkingCar("C2", "");
            Space s2 = assigner.assignSpace(C2);
            Assert.That(s2.getDesirability(), Is.EqualTo(120));
            assigner.onSpaceTaken(C2, s2);

            assigner.onSpaceFreed(C2, parkingLots[0]);
            assigner.onSpaceFreed(C1, s2);
            assigner.onGarageExit(C1);
            assigner.onGarageExit(C2);
        }
    }
}
