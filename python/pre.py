import numpy as np
import pandas as pd
import sys, os, vtk, copy
from scipy.interpolate import griddata
from csv import reader
import matplotlib.pyplot as plt

INTERP_TYPE = 'LS linear'

class odmodel:
    
    def __init__(self):
        print("odmodel init")
        self._screwfilename = ''
        self._dbfilename = ''
        self._srdbfilename = ''
        self._interp_type = ''
        self.rpm_c = -100.0
        self._flowrate = 150
        self.tin = 20
        self.tsun = 20
        self.tring1 = 50
        self.tring2 = 50
        self.tring3 = 50
        self.spindles_types = []
        self.spindles_num = []
        self.stopring_diam = []
        self.results = {}

    def generate_results(self,machine):
        print("generate 1D fields")
        buf, pressure = machine.get_pressure()
        buf, temp = machine.get_temperature()
        self.results['z'] = buf
        buf, phi = machine.get_phi()
        buf, pinch = machine.get_pinch()
        buf, pinch_label = machine.get_pinch_label()
        self.results['P [bar]'] = np.array(pressure)/1.0e+5
        self.results['T [°C]'] = np.array(temp) - 273
        self.results['Phi'] = np.array(phi)
        self.results['ModuleLabel'] = np.array(pinch_label)
        self.results['Module'] = np.array(pinch)/(len(np.unique(pinch)) - 1)

    
    def read_db(self):
        """Read the csv of the PRE database"""
        # assuming data.csv is a CSV file with the 1st row being the names for the columns
        # read the file
        self.db = pd.read_csv(self._dbfilename, encoding='unicode_escape', sep=',').fillna(-1)
        self.db = self.db.apply(pd.to_numeric, errors='ignore')

    def read_screw(self):
        """Read the device setup"""
        # open file in read mode
        with open(self._screwfilename, 'r') as read_obj:
            # pass the file object to reader() to get the reader object
            csv_reader = reader(read_obj)
            header = next(csv_reader)[0].split(';')
            phi_mod_descr = []
            self.mixer_descr = []
            # Iterate over each row in the csv using reader object
            for row in csv_reader:
                # row variable is a list that represents a row in csv
                loc_el = dict(zip(header, row[0].split(';')))
                phi_mod_descr.append(copy.deepcopy(loc_el))
                # at each stopring reset module
                if loc_el['type'] == 'SR':
                    self.mixer_descr.append(copy.deepcopy(phi_mod_descr))
                    phi_mod_descr = []

    #####################################################
    #   class for working condition of physical module
    #####################################################
    class process_parameter():
        def __init__(self, vel, q, tsun, tring):
            """
            Vel angular velocity rpm
            Q flow rate Kg/h
            TSun sun temperature K
            TRing ring temperature K
            "NPlanets", "PlanetsType", "TIn", "RPM", "MassFlowRate", "TSun", "TRing"
            """
            self.props = {
                'RPM': vel, 'MassFlowRate': q,
                'TSun': tsun, 'TRing': tring, 'vdot':q/3600.0/1200.0,
                'cp': lambda tb: 1561 + 2.6*tb, 'rho':1200
            }
        def get_coord(self):
            return [self.props[d] for d in ['RPM', 'MassFlowRate', 'TSun', 'TRing']]



    #################################################################
    #   Physical module generates linked numerical modules
    #################################################################
    class phys_mod():
        """Physical module, set of numerical modules"""
        class num_mod():
            """Numerical module"""
            class interpolator_op:
                """Interpolation of the database"""
                def __init__(self, df, interp_type="Unstructured linear"):
                    self.df = df
                    self.coord_labels = ["TIn", "RPM", "MassFlowRate", "TSun", "TRing"]
                    self.coord = df[self.coord_labels].to_numpy()
                    self.values = {}
                    for i in self.coord_labels:
                        self.values[i] = df[i].to_numpy()

                def interp_safe (self, varname, point):
                    """Robust interpolation: nearest neighbour"""
                    return griddata(self.coord, self.df[varname].to_numpy(), point, method='nearest')[0]

                def interp (self, varname, point):
                    """Interpolation of the database"""
                    global INTERP_TYPE
                    if INTERP_TYPE == "Unstructured linear":
                        value = griddata(self.coord, self.df[varname].to_numpy(), point, method='linear')[0]
                        if np.isnan(value):
                            for i,p in enumerate(point):
                                minc, maxc = self.coord[:, i].min(), self.coord[:, i].max()
                                if p < minc or p > maxc:
                                    break
                            #print("\n------------->     WARNING       <-------------")
                            #print("out of database range for variable", self.coord_labels[i])
                            #print("   value =", p, "   min =", minc, "   max =", maxc)
                            #print("------------->     WARNING       <-------------\n")
                            value = self.interp_safe(varname, point)
                        return value

                    else:
                        loc_db_p = self.df[varname].to_numpy()

                        loc_p = -1

                        A = np.ones((loc_db_p.shape[0], 1))
                        xx = self.coord_labels
                        point2 = [1]
                        for i,xi in enumerate(xx):
                            A = np.hstack((A, np.c_[self.values[xi]]))
                            point2.append(point[i])

                        if INTERP_TYPE == 'LS bilinear' or INTERP_TYPE == 'LS quadratic':
                            for i,xi in enumerate(xx[:-1]):
                                for j,xj in enumerate(xx[(i + 1):]):
                                    A = np.hstack((A, np.c_[self.values[xi]*self.values[xj]]))
                                    point2.append(point[i]*point[i + 1 + j])
                            if INTERP_TYPE == 'LS quadratic':
                                for xi in xx:
                                    A = np.hstack((A, np.c_[self.values[xi]**2]))
                                    point2.append(point[i]**2)

                        y = np.r_[loc_db_p]
                        try:
                            beta_sol = np.linalg.lstsq(A, y, rcond=None)
                            beta = beta_sol[0]
                            y_hat = np.dot(point2, beta)
                            return y_hat
                        except:
                            print('Skip kernel interpolation')
                            return np.nan


            def __init__(self, num_type, points, prev_mod, phys_mod, npl=None, plt=None, verbose=False):
                self.verbose = verbose
                # type of numerical module: mixer or stopring
                self.type = num_type
                self.nplanets = npl
                self.planet_type = plt
                # points associated to the numerical module
                self.pts = points
                self.dx = np.diff(points)[0]
                self.u = 0.0 # velocity{pt_0}
                self.T = 0.0 # Temperature{pt_0}
                self.phi = 0.0 # filling factor{pt_1}
                self.gradp = 0.0 # pressure gradient
                self.tot_gradp = 0.0 # pressure gradient
                self.p = 0.0 # pressure gradient
                self.qt = 0.0 # local viscous heating
                self.q0 = 0.0 # local natural flux
                self.prev = prev_mod # pointer to the previous
                self.next = None     #pointer to the next
                self.phys_mod = phys_mod
                self.update_T = {'mix': self.__energy_balance, 'stop': self.__add_dtemp}
                self.update_p = self.__add_dp
                self.update_phi = self.__compute_phi

            def set_db(self, df):
                if self.nplanets is not None:
                    idxs = \
                        (np.int64(df['NPlanets']) == np.int64(self.nplanets)) \
                       *(df['PlanetsType'] == self.planet_type)
                    df = df[idxs]
                if df.shape[0] == 0:
                    raise RuntimeError("Combination of %d planets of type %s not found" % (int(self.nplanets), self.planet_type))
                self.interpolator = self.interpolator_op(df)

            def __energy_balance(self, t_prev):
                if self.verbose: print('compute energy balance', end=" ")
                interp_point = [t_prev] +  self.phys_mod.wc.get_coord()

                # Get Delta T: DEPRECATED
                #dt = self.interpolator.interp('delta_t',interp_point)

                # Get viscous heating
                # vh_dt = self.interpolator.interp('VH', interp_point)
                vh_dt = self.dx*self.interpolator.interp('VHspec', interp_point)
                #tbulk = self.interpolator.interp('TBulk', interp_point)

                # Get thermal properties
                vdot = self.phys_mod.wc.props['vdot']
                cp = self.phys_mod.wc.props['cp']
                rho = self.phys_mod.wc.props['rho']
                mdot = rho*vdot
                # Temperature BCs
                t_barr = self.phys_mod.wc.props['TRing']
                t_shaft = self.phys_mod.wc.props['TSun']
                # Surface area
                a_barr = 6.0*1.56e-3
                a_shaft = 6.0*7e-4
                # Thermal diffusivity Wm^2/K
                alpha_barr = 100.0
                alpha_shaft = 100.0

                # print(self.interpolator.df['inflow'][0],self.interpolator.df['Q'][0])
                # q_in = self.interpolator.df
                # tavg=(self.T+t_prev)/2
                tavg = (self.T)
                qh_barr = alpha_barr*a_barr*(t_barr - tavg)
                qh_shaft = alpha_shaft*a_shaft*(t_shaft - tavg)
                dt_eb = (vh_dt + qh_barr + qh_shaft)/(mdot*cp(tavg)*6)

                # # Considering T bulk
                # tbulkout = vh_dt + alpha_barr*a_barr*(t_barr - t_prev/2) \
                #     + alpha_shaft*a_shaft*(t_shaft - t_prev/2) + mdot*cp(tbulk)*t_prev
                # tbulkout /= (alpha_barr*a_barr/2 + alpha_shaft*a_shaft/2 + mdot*cp(tbulk))
                # dt_eb = tbulkout - t_prev


                # dt=(Q_ax(i)+Q_alb(i)+Q_barr(i))/(Q_in(1)*6*rho*cp); %deltaT ricavato da un bilancio termico sul modulo numerico
                #T(i+1)=T(i)+Phi(i)*DeltaT(i); %l'aumento di temperatura può essere o meno pesato con il fattore di riempimento
                # print('barr qh ' , qh_barr )
                # print('sh ' , qh_sh )
                # print("viscous heating", dt_eb, " delta t ", dt)

                self.T = t_prev + dt_eb*self.phi
                if self.verbose: print(dt_eb, dt_eb*self.phi, t_prev, self.T)

            def __add_dtemp(self, t_prev):
                if self.verbose: print('add delta Temperature', end=" ")
                interp_point = [t_prev] + self.phys_mod.wc.get_coord()
                dt = self.interpolator.interp('deltaTBulk', interp_point)
                self.T = t_prev + dt
                if self.verbose: print(dt, t_prev, self.T)

            def __add_dp(self, p_prev):
                if self.verbose: print('add delta Pressure', end=" ")
                interp_point = [self.T] + self.phys_mod.wc.get_coord()
                if self.type == 'stop':
                    dp = self.interpolator.interp('deltaP', interp_point)
                else:
                    dp = self.dx*self.interpolator.interp('gradP', interp_point)
                new_p = p_prev - dp
                # check if the pressure is still positive
                self.tot_gradp = dp
                if (new_p > 0):
                    self.p = new_p
                    self.gradp = dp
                else:
                    self.p = 0
                    self.gradp = p_prev
                if self.verbose: print(dp, p_prev, new_p, self.p)

            def __compute_phi(self):
                if self.verbose: print('compute filling factor', end=" ")
                interp_point = [self.T] +  self.phys_mod.wc.get_coord()
                if self.type == 'stop':
                    if self.verbose: print('stopring', end=" ")
                    self.phi = 1
                    self.q0 = 0
                else:
                    q0 = self.interpolator.interp('Q0', interp_point)
                    self.q0 = q0
                    self.phi = self.gradp/self.tot_gradp \
                        + (1 - self.gradp/self.tot_gradp)*self.phys_mod.wc.props['vdot']/q0
                if self.verbose: print(self.phi)

        class full_db:
            """Filter database for the single module"""
            def __init__(self, stopring_db, simulations_db, sr_geom):
                # print("Reading dbs from\n%s\n%s"  % (stopring_db, simulations_db))
                buf = pd.read_csv(stopring_db)
                buf = buf.apply(pd.to_numeric, errors='ignore')
                #select the correct size of mixer
                if 'SRDiam' in buf.columns:
                    self.sr = copy.deepcopy(buf[(buf['SRDiam'] > float(sr_geom)-0.1) & (buf['SRDiam'] < float(sr_geom)+0.1)])
                elif 'Geom' in buf.columns:
                    self.sr = copy.deepcopy(buf[(buf['Geom'] > float(sr_geom)-0.1) & (buf['Geom'] < float(sr_geom)+0.1)])
                self.mixer = pd.read_csv(simulations_db, encoding='unicode_escape', sep=',').fillna(-1)
                self.mixer = self.mixer.apply(pd.to_numeric, errors='ignore')

        def __init__(self, name, wc, descr, shift=0):
            self.name = name
            n_step = len(descr)
            self.wc = wc  # working conditions
            prev_mod = None
            for i in range(n_step - 1):
                npli = descr[i]['nsat']
                plti = descr[i]['type']
                dx = float(descr[i]['l'])
                x0 = shift if prev_mod is None else prev_mod.pts[1]
                el = self.num_mod('mix', [x0, x0 + dx], prev_mod, self, npl=npli, plt=plti)
                prev_mod = el
            i += 1
            dx = float(descr[i]['l'])
            self.sr_size = int(descr[i]['size'])
            x0 = prev_mod.pts[1]
            el = self.num_mod('stop', [x0, x0 + dx], prev_mod, self)
            self.last = el
            next_el = None
            while(el.prev):
                el.next = next_el
                next_el = el
                el = el.prev
            el.next = next_el
            self.first = el
            el = self.last
            while(el.prev):
                el = el.prev
            self.length = self.last.pts[-1] + self.first.pts[0]


        def set_db(self, namestop, namemixer):
            self.db = []
            self.db = self.full_db(namestop, namemixer, self.sr_size)
            el = self.first
            go_on = True
            selector = {'mix': self.db.mixer, 'stop': self.db.sr }
            while(go_on):
                # Print only to chek the correct chain
                # print(el.type, el.phys_mod.name, el.pts[0])
                #To do add database based on el.satType
                el.set_db(selector[el.type])
                go_on = el.next
                el = el.next

    ##########################################
    # mixer class gather physical modules
    ##########################################
    class mixer():
        def __init__(self,phys_mds): #ordere phisical modules
            for idx, imod in enumerate(phys_mds):
                try:
                    imod_pp = phys_mds[idx+1] # next physical module
                    imod.last.next = imod_pp.first
                    imod_pp.first.prev = imod.last
                except:
                    print("end physical modules")
                self.first = phys_mds[0].first
                self.last = phys_mds[-1].last
    #method to set bc
        def set_BC(self,t_in,p_out):
            self.t_in = t_in
            self.p_out = p_out
    #method to inital coditions for temperature
        def set_initial(self,t_0,phi_0):
            el = self.first
            go_on=True
            while(go_on):
                el.T=t_0
                el.phi = phi_0
                go_on = el.next
                el = el.next

    #method to update Temperature from firts to last
        def update_T(self):
            el = self.first
            go_on=True
            t_prev = self.t_in
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                el.update_T[el.type](t_prev)
                # if (el.T>411.): el.T=411.
                t_prev = el.T
                go_on = el.next
                el = el.next
    #method to update Pressure from last to frame
        def update_P(self): # -> solve P
            el = self.last
            go_on=True
            p_prev = self.p_out
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[-1])
                el.update_p(p_prev)
                p_prev = el.p
                go_on = el.prev
                el = el.prev

        # method to update Pressure from last to frame
        def update_phi(self): # -> solve P
            el = self.last
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[-1])
                el.update_phi()
                go_on = el.prev
                el = el.prev

        def get_temperature(self):
            temp=[self.t_in]
            el = self.first
            x=[el.pts[0]]
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                temp.append(el.T)
                x.append(el.pts[1])
                go_on = el.next
                el = el.next
            return [x,temp]

        def get_pressure(self):
            p=[]
            x=[]
            el = self.first
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                p.append(el.p)
                x.append(el.pts[0])
                go_on = el.next
                el = el.next
            p.append(self.p_out)
            x.append(self.last.pts[-1])
            return [x,p]

        def get_phi(self):
            res=[]
            x=[]
            el = self.first
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                res.append(el.phi)
                x.append(el.pts[1])
                go_on = el.next
                el = el.next
            return [x,res]

        def get_pinch_label(self):
            res=[]
            x=[]
            el = self.first
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                res.append(el.planet_type)
                x.append(el.pts[0])
                go_on = el.next
                el = el.next
            return [x,res]

        def get_pinch(self):
            type_dict = {}
            count = 0
            res=[]
            x=[]
            el = self.first
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                if type_dict.get(el.planet_type) is None:
                    type_dict[el.planet_type] = count
                    count += 1
                res.append(type_dict[el.planet_type])
                x.append(el.pts[0])
                go_on = el.next
                el = el.next
            return [x,res]


        def get_q0(self):
            res=[]
            x=[]
            el = self.first
            go_on=True
            while(go_on):
                # print(el.type, el.phys_mod.name, el.pts[0])
                res.append(el.q0)
                x.append(el.pts[0])
                go_on = el.next
                el = el.next
            return [x,res]

        def plot(self):
            fig, ax = plt.subplots()
            [t, s]=self.get_temperature()
            ax.plot(t, s)
            fig, ax = plt.subplots()
            [t, s]=self.get_pressure()
            ax.plot(t, s)
            fig, ax = plt.subplots()
            [t, s]=self.get_phi()
            ax.plot(t, s)
            fig, ax = plt.subplots()
            ax.set_title('Q0')
            [t, s]=self.get_q0()
            ax.plot(t, s)
            plt.show()

        def plot_convergence(self):
            fig, ax = plt.subplots()
            [t, s]=self.get_temperature()
            for i,itemp in enumerate(self.converge_t):
                ax.plot(t,itemp,label = str(i))
            ax.legend()
            fig, ax = plt.subplots()
            [t, s]=self.get_pressure()
            for i,itemp in enumerate(self.converge_p):
                ax.plot(t,itemp,label = str(i))
            ax.legend()

        def solve(self, tol=1.0e-03, maxit=1000):
            # relative error temperature
            rel_er_t = 1.0
            norm_t = 1.0
            norm_t_old = 1.0
            # relative error pressure
            rel_er_p = 1.0
            norm_p = 1.0
            norm_p_old = 1.0
            # convergence statistics
            self.converge_t = []
            self.converge_p = []
            it = 0
            # iterative loop
            while (rel_er_t > tol or rel_er_p > tol) and (it < maxit):
                print("iteration %d" % it, "   pressure error %1.03g" % rel_er_p, "   temperature error %1.03g" % rel_er_t)
                # perform computations
                self.update_T()
                self.update_P()
                self.update_phi()
                # Check convergence
                xx, temp = self.get_temperature()
                xx, pressure = self.get_pressure()
                if it > 0:
                    rel_er_t = np.linalg.norm(np.array(temp) - self.converge_t[-1])/(norm_t_old + 1.0e-12)
                    rel_er_p = np.linalg.norm(np.array(pressure) - self.converge_p[-1])/(norm_p_old + 1.0e-12)
                # record old field
                self.converge_t.append(np.array(temp))
                self.converge_p.append(np.array(pressure))
                # compute old field norm
                norm_t_old = np.linalg.norm(self.converge_t[-1])
                norm_p_old = np.linalg.norm(self.converge_p[-1])
                # advance: it += 1
                it = len(self.converge_t)


    def RequestData(self):
        print("REQUEST DATA")
        global INTERP_TYPE
        print("Using interpolation", INTERP_TYPE)
        # self.read_db()
        self.read_screw()
        ph_m = []
        for count,imod in enumerate(self.mixer_descr):
            # TODO: different planets
            tring = 0
            if count == 0:
                tring = self.tring1
            elif count == 1:
                tring = self.tring2
            elif count == 2:
                tring = self.tring3
            else:
                print("WARNING: a maximum of three modules is allowed for temperature of ring")
                print("Setting temperature of modules > 3 to temperature of module 3")
                tring = self.tring3
            wc = self.process_parameter(
                vel = self.rpm_c, q = self._flowrate,
                tsun = self.tsun + 273.0, tring = tring + 273.0
            )
            if count == 0:
                ph_m.append(
                    self.phys_mod(
                        'module %d' % count, copy.deepcopy(wc), imod
                    )
                )
            else:
                ph_m.append(
                    self.phys_mod(
                        'module %d' % count, copy.deepcopy(wc),
                        imod, shift=ph_m[count-1].last.pts[-1]
                    )
                )

        print('Setting db using\n%s\n%s' % (self._dbfilename, self._srdbfilename))
        for ipm in ph_m:
            ipm.set_db(self._srdbfilename, self._dbfilename)
        pre = self.mixer(ph_m)
        pre.set_BC(t_in=self.tin + 273.0, p_out=0.0)
        pre.set_initial(t_0=self.tin + 273.0, phi_0=0.5)
        pre.solve(1.0e-06, maxit=100)

        self.generate_results(pre)
        pre.plot()
        print('FINE LAVORAZIONE')
        return 1


if __name__ == "__main__":
    od1 = odmodel()

    #Specify filename for PRE database#
    od1._dbfilename = 'db_modules.csv'

    #Specify filename for the stopring database#
    od1._srdbfilename = 'db_stopring.csv'

    #Specify filename for PRE setup#
    od1._screwfilename = 'machine.csv'

    #Select interpolation type: ["Unstructured linear", "LS linear", "LS bilinear", "LS quadratic"] #
    od1._interp_type = "LS linear"
    # global INTERP_TYPE

    #Specify device RPM min=50, max=250#
    od1.rpm_c = 100

    #Specify inflow flow rate Kg/h min=50, max=400#
    od1._flowrate = 200

    #Specify inflow temperature °C min=15, max=100#
    od1.tin = 30

    #Specify central shaft temperature °C min=15, max=100#
    od1.tsun = 20

    #Specify barrel 1 temperature °C min=25, max=150#
    od1.tring1 = 50

    #Specify barrel 2 temperature °C min=25, max=150#
    od1.tring2 = 50

    #Specify barrel 3 temperature min=25, max=150#
    od1.tring3 = 50

    # Solve the problem
    od1.RequestData()
