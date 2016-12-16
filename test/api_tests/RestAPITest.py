import json
import requests
import subprocess
import unittest
from time import sleep

DEL_USERS_CONTACTS = "/users/contacts"
DEL_USERS_ME = "/users/me"
DEL_USERS_RECOMMEND = "/users/recommend"
GET_LOGIN = "/login"
GET_USERS = "/users"
GET_USERS_CONTACTS = "/users/contacts"
GET_USERS_ME = "/users/me"
GET_USERS_SEARCH = "/users/search"
POST_SIGNUP = "/signup"
POST_USERS_CONTACTS = "/users/contacts"
POST_USERS_ME = "/users/me"
POST_USERS_RECOMMEND = "/users/recommend"
DEL_CHATS = "/chats"
DEL_MESSAGES = "/messages"
GET_CHATS = "/chats"
POST_CHATS = "/chats"
GET_CATEGORIES = "/categories"
GET_JOB_POSITIONS = "/job_positions"
GET_SKILLS = "/skills"
DELETE_ALLBDS = "/bds/delete"





# ~ import responses
URL = 'http://localhost:8000'

class Client():
    def get_standard(self, endpoint, token = "", params = {}):
        response = requests.get(URL + endpoint, headers={'Token': token}, params=params)
        return response.status_code, json.loads(response.text)

    def get_login(self, usr, psw):
        response = requests.get(URL + GET_LOGIN, auth=(usr, psw))
        return response.status_code, json.loads(response.text)

    def post_standard(self, endpoint, token = "", payload = {}):
        r = requests.post(URL + endpoint, headers={'Token': token}, data=json.dumps(payload))
        return r.status_code, json.loads(r.text)

    def post_signup(self, usr, psw, payload):
        r = requests.post(URL + POST_SIGNUP, auth=(usr, psw), data=json.dumps(payload))
        return r.status_code, json.loads(r.text)

    def delete_standard(self, endpoint, token = "",payload = {}):
        r = requests.delete(URL + endpoint, headers={'Token': token}, data=json.dumps(payload))
        return r.status_code, json.loads(r.text)






class TestCase(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        print "Inicio Servidor"
        p = subprocess.Popen(["./../../build/Jobify_AppServer", "8000", "-d", "-t"])
        # p = subprocess.Popen(["./Jobify_AppServer", "8000", "-d", "-t"], stdout=subprocess.PIPE)
        print p.pid
        cls.server_pid = p.pid
        sleep(3) # time to open the server before tests begin

    @classmethod
    def tearDownClass(cls):
        print "Termino Servidor"
        subprocess.call(["kill", "-2", str(cls.server_pid)])

    #@responses.activate
    def setUp(self):
        self.client = Client()
        self.tearDown()
        self.checkEmptyBD()

    def tearDown(self):
        #eliminar la base de datos. /bds/delete admin:admin
        response = requests.delete(URL + DELETE_ALLBDS, auth=("admin", "admin"))
        self.assertEqual(200, response.status_code)
        self.assertEqual((json.loads(response.text))["status"], "succes")

    def checkEmptyBD(self):
        # Chequear que este vacio
        code, body = self.client.get_standard(GET_USERS)
        expected = {
            "data": [],
            "status": "succes"
        }
        self.assertEqual(expected, body)
        self.assertEqual(200, code)

    def add_user(self, user):
        code, body = self.client.post_signup(user["email"],"1234",user)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], "succes")
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]
        return code, body

    def test_add_user_ale(self):
        expected = {
            "data": {
                "token" : ""
            },
            "status": "succes"
        }

        default_user = generate_user("ale")
        code, body = self.client.post_signup("ale","1234",default_user)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]

    def test_get_user_gabi(self):
        user = generate_user("gabi")
        self.add_user(user)
        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        body_user = body["data"]
        for key, value in user.items():
            self.assertEqual(value,body_user[key])

    def test_login_wrong_password(self):
        expected = {
            "data": "",
            "error": {
                "code": 401,
                "message": "Invalid password or user"
            },
            "status": "error"
        }
        code, body = self.client.get_login("ale","4321")
        self.assertEqual(code,expected["error"]["code"])
        self.assertEqual(body["error"], expected["error"])

    def test_login_ale(self):
        user = generate_user("ale")
        self.add_user(user)
        expected = {
            "data": {
                "token" : ""
            },
            "status": "succes"
        }
        sleep(1)
        code, body = self.client.get_login("ale","1234")
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]

    def test_get_contacts(self):
        ale = generate_user("ale")
        gabi = generate_user("gabi")
        juan = generate_user("juan")
        gabi["contacts"].append("ale")
        gabi["contacts"].append("juan")
        self.add_user(ale)
        self.add_user(juan)
        self.add_user(gabi) #me quedo con el token de gabi

        code, body = self.client.get_standard(GET_USERS_CONTACTS, self.token)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], "succes")
        # print body["data"]
        # self.assertItemsEqual([ale,juan], body["data"])

        for user in body["data"]:
            self.assertTrue(user["username"] in ["ale","juan"])

    #DEL_USERS_ME = "/users/me"
    def test_delete_user(self):
        ale = generate_user("ale")
        gabi = generate_user("gabi")
        juan = generate_user("juan")

        self.add_user(ale)
        self.add_user(juan)
        self.add_user(gabi) #me quedo con el token de gabi

        code, body = self.client.get_standard(GET_USERS)
        expected = {
            "data": [ale,gabi,juan],
            "status": "succes"
        }
        self.assertEqual(200, code)
        self.assertEqual(body["status"], expected["status"])
        for user in body["data"]:
            self.assertTrue(user in expected["data"])

        code, body = self.client.delete_standard(DEL_USERS_ME,self.token)
        self.assertEqual(200, code)
        self.assertEqual(body["status"], expected["status"])

        code, body = self.client.get_standard(GET_USERS)
        expected = {
            "data": [ale,juan],
            "status": "succes"
        }
        self.assertEqual(200, code)
        self.assertEqual(body["status"], expected["status"])
        for user in body["data"]:
            self.assertTrue(user in expected["data"])
        self.assertEqual(len(body["data"]),2)



    def test_get_users_search_ordered_by_recommendations(self):
        ale = generate_user("ale")
        gabi = generate_user("gabi")
        juan = generate_user("juan")

        ale["recommended_by"].append("ale");
        ale["recommended_by"].append("ale");

        gabi["recommended_by"].append("pepe");
        gabi["recommended_by"].append("ale");
        gabi["recommended_by"].append("gabi");
        gabi["recommended_by"].append("ale");

        juan["recommended_by"].append("ale");
        juan["recommended_by"].append("ale");
        juan["recommended_by"].append("ale");

        self.add_user(ale)
        self.add_user(juan)
        self.add_user(gabi) #me quedo con el token de gabi

        #sort=recommended&filter=10&job_position=developer&skill=java&distance=100
        params = {'sort':'recommended_by','filter':'10','job_position':'Desarrollador','skill':'C','distance':'100'}
        code, body = self.client.get_standard(GET_USERS_SEARCH, self.token, params)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], "succes")
        self.assertEqual(body["data"][0]["username"], "gabi")
        self.assertEqual(body["data"][1]["username"], "juan")
        self.assertEqual(body["data"][2]["username"], "ale")

    #POST_USERS_CONTACTS
    def test_post_contact(self):
        ale = generate_user("ale")
        gabi = generate_user("gabi")
        self.add_user(gabi)
        self.add_user(ale)
        expected = {
            "data": "ok",
            "status": "succes"
        }
        payload = {'username':'gabi'}
        code, body = self.client.post_standard(POST_USERS_CONTACTS,self.token,payload)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertEqual(body["data"], "ok")

        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        body_user = body["data"]
        self.assertEqual(body["data"]["contacts"], ["gabi"])

    def test_post_contact_inexistent(self):
        ale = generate_user("ale")
        self.add_user(ale)
        expected = {
            "data" : "",
            "error" :
                {
                    "code" : 500,
                    "message" : "Internal server error"
                },
            "status" : "error"
        }
        payload = {'username':'gabi'}
        code, body = self.client.post_standard(POST_USERS_CONTACTS,self.token,payload)
        self.assertEqual(code,expected["error"]["code"])
        self.assertEqual(body["status"], expected["status"])
        self.assertEqual(body["data"], expected["data"])

    # DEL_USERS_CONTACTS = "/users/contacts"
    def test_del_contact(self):
        self.test_post_contact()
        expected = {
            "data": "ok",
            "status": "succes"
        }
        payload = {'username':'gabi'}
        code, body = self.client.delete_standard(DEL_USERS_CONTACTS, self.token, payload)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertEqual(body["data"], "ok")

        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        body_user = body["data"]
        self.assertEqual(body["data"]["contacts"], [])




    #POST_USERS_ME
    def test_edit_user_me(self):
        ale = generate_user("ale")
        self.add_user(ale)
        expected = {
            "data": "ok",
            "status": "succes"
        }
        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        self.assertEqual(body["data"]["city"], "Rosario")
        body_user = body["data"]
        body_user["city"] = "Buenos Aires"
        payload = body_user

        code, body = self.client.post_standard(POST_USERS_ME,self.token,payload)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertEqual(body["data"], "ok")

        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        body_user = body["data"]
        self.assertEqual(body["data"]["city"], "Buenos Aires")

    #POST_USERS_RECOMMEND
    def test_post_recommend_user(self):
        ale = generate_user("ale")
        gabi = generate_user("gabi")
        self.add_user(gabi)
        self.add_user(ale)
        expected = {
            "data": "ok",
            "status": "succes"
        }
        payload = {'username':'gabi'}
        code, body = self.client.post_standard(POST_USERS_RECOMMEND,self.token,payload)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertEqual(body["data"], "ok")

        #me logue con gabi
        sleep(1)
        code, body = self.client.get_login("gabi","1234")
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]

        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        self.assertEqual(body["data"]["contacts"], [])
        self.assertEqual(body["data"]["recommended_by"], ["ale"]) #gabi esta recomendado por ale

    def test_del_recommend_user(self):
        self.test_post_recommend_user()
        expected = {
            "data": "ok",
            "status": "succes"
        }

        #me logueo con ale
        sleep(1)
        code, body = self.client.get_login("ale","1234")
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]


        payload = {'username':'gabi'}
        code, body = self.client.delete_standard(DEL_USERS_RECOMMEND, self.token, payload)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertEqual(body["data"], "ok")

        #me logue con gabi
        sleep(1)
        code, body = self.client.get_login("gabi","1234")
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]

        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        body_user = body["data"]
        self.assertEqual(body["data"]["recommended_by"], [])
    #GET_CATEGORIES = "/categories"
    # GET_JOB_POSITIONS = "/job_positions"
    # GET_SKILLS = "/skills"
    def test_get_shared_categories(self):
        code, body = self.client.get_standard(GET_CATEGORIES)
        expected = {
            "data": ["stufffff"],
            "status": "succes"
        }
        self.assertEqual(expected["status"], body["status"])
        self.assertEqual(200, code)
        self.assertIsNotNone(body["data"])

    def test_get_shared_job_positions(self):
        code, body = self.client.get_standard(GET_JOB_POSITIONS)
        expected = {
            "data": ["stufffff"],
            "status": "succes"
        }
        self.assertEqual(expected["status"], body["status"])
        self.assertEqual(200, code)
        self.assertIsNotNone(body["data"])

    def test_get_shared_skills(self):
        code, body = self.client.get_standard(GET_SKILLS)
        expected = {
            "data": ["stufffff"],
            "status": "succes"
        }
        self.assertEqual(expected["status"], body["status"])
        self.assertEqual(200, code)
        self.assertIsNotNone(body["data"])



def generate_user(username):
    return {
        "chats" : [],
        "city" : "Rosario",
        "contacts" : [],
        "coordenates" : "",
        "dob" : "1993-08-19",
        "email" : username,
        "firebase_token" : "",
        "gender" : "male",
        "name" : "Alejandro Pablo Levinas",
        "nationality" : "argentino",
        "previous_exp" :
            [
                {
                    "company" : "NASA",
                    "description" : "Desarrollador en lenguaje R para analizar......",
                    "position" : "Desarrollador",
                    "years" : "2006-2009"
                },
                {
                    "company" : "UBA",
                    "description" : "Docente de la materia Taller 2",
                    "position" : "Docente",
                    "years" : "2010-actualidad"
                }
            ],
        "profile" : "Soy un estudiante de ingenieria en informatica que se propone............blabllbla...........",
        "recommended_by" : [],
        "skills" :
            [
                "C",
                "C++",
                "GoogleTest"
            ],
        "username" : username
    }

if __name__ == '__main__':
    unittest.main()

