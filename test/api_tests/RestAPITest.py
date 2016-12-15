import json
import requests
import unittest

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
    def get_standard(self, endpoint, token = ""):
        response = requests.get(URL + endpoint, headers={'Token': token})
        return response.status_code, json.loads(response.text)

    def get_login(self, usr, psw):
        response = requests.get(URL + GET_LOGIN, auth=(usr, psw))
        return response.status_code, json.loads(response.text)

    def post_signup(self, usr, psw, payload):
        r = requests.post(URL + POST_SIGNUP, auth=(usr, psw), data=json.dumps(payload))
        return r.status_code, json.loads(r.text)





class TestCase(unittest.TestCase):
    # @responses.activate
    def setUp(self):
        self.client = Client()
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
                "token" : "1111111111111111111111"
            },
            "status": "succes"
        }

        default_user = generate_user("ale");
        code, body = self.client.post_signup("ale","1234",default_user)
        self.assertEqual(code,200)
        self.assertEqual(body["status"], expected["status"])
        self.assertIsNotNone(body["data"]["token"])
        self.token = body["data"]["token"]

    def test_get_user_gabi(self):
        user = generate_user("gabi");
        self.add_user(user);
        code, body = self.client.get_standard(GET_USERS_ME, self.token)
        self.assertEqual(code,200)
        body_user = body["data"]
        for key, value in user.items():
            self.assertEqual(value,body_user[key])


'''
    def CategoryRequestInsert(self, name, description):
        payload = {'category': {'name': name, 'description': description}}
        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
        return requests.post(URL + '/categories', data=json.dumps(payload), headers=headers), payload

    def CategoryInsertSimple(self, name, description):
        response, payload = self.CategoryRequestInsert(name, description)
        self.assertEqual(json.dumps(payload), json.dumps(json.loads(response.text)))
        self.assertEqual(201, response.status_code)

    def CategoryInsertSimpleExpectedError(self, name, description):
        response, payload = self.CategoryRequestInsert(name, description)
        self.assertEqual(500, response.status_code)

    def CategoryDeleteSimple(self, name):
        response = requests.delete(URL + '/categories/' + name)
        self.assertEqual(204, response.status_code)

    def getCategories(self):
        return requests.get(URL + '/categories')

    def updateCategory(self, oldName, newName, newDescription):
        payload = {'category': {'name': newName, 'description': newDescription}}
        headers = {'Content-type': 'application/json', 'Accept': 'text/plain'}
        response = requests.post(URL + '/categories/' + oldName, data=json.dumps(payload), headers=headers)
        self.assertEqual(json.dumps(payload), json.dumps(json.loads(response.text)))
        self.assertEqual(201, response.status_code)

    def testCategoryInsertAndDelete(self):
        self.checkEmptyBD()

        # Agregar categoria 1
        self.CategoryInsertSimple('sport', 'sport activities')

        response = self.getCategories()
        espected = {"categories": [{"name": 'sport', "description": 'sport activities'}], "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminamos categoria 1
        self.CategoryDeleteSimple('sport')

        self.checkEmptyBD()

    def testCategoryIntertTwoDeleteTwo(self):
        self.checkEmptyBD()

        # Agregar categoria 1
        self.CategoryInsertSimple('software', 'software activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'}],
                    "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Agregar categoria 2
        self.CategoryInsertSimple('administration', 'administration activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'},
                                   {"name": 'administration', "description": 'administration activities'}],
                    "metadata": {"count": 2}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminamos categoria 2
        self.CategoryDeleteSimple('administration')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'}],
                    "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminamos categoria 1
        self.CategoryDeleteSimple('software')

        self.checkEmptyBD()

    def testInsertTwiceGetError(self):
        self.checkEmptyBD()

        # Agregar categoria 1
        self.CategoryInsertSimple('software', 'software activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'}],
                    "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Agregar categoria 1
        self.CategoryInsertSimpleExpectedError('software', 'software activities')

    def testUpdateCategory(self):
        self.checkEmptyBD()

        # Agregar categoria 1
        self.CategoryInsertSimple('sport', 'sport activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'sport', "description": 'sport activities'}], "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Actualizar categoria 1
        self.updateCategory('sport', 'outdoor activies', 'all kind of outdoor activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'outdoor activies', "description": 'all kind of outdoor activities'}],
                    "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminar categoria 1
        self.CategoryDeleteSimple('outdoor activies')

        self.checkEmptyBD()

    def testInsertDeleteUpdateMixed(self):
        self.checkEmptyBD()

        # Agregar categoria 1
        self.CategoryInsertSimple('software', 'software activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'}],
                    "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Agregar categoria 2
        self.CategoryInsertSimple('administration', 'administration activities')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'},
                                   {"name": 'administration', "description": 'administration activities'}],
                    "metadata": {"count": 2}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Agregar categoria 3
        self.CategoryInsertSimple('music', 'all kind of music')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'},
                                   {"name": 'administration', "description": 'administration activities'},
                                   {"name": 'music', "description": 'all kind of music'}], "metadata": {"count": 3}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminamos categoria 2
        self.CategoryDeleteSimple('administration')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'},
                                   {"name": 'music', "description": 'all kind of music'}], "metadata": {"count": 2}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Actualizar categoria 3
        self.updateCategory('music', 'classic music', 'only classic music')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'software', "description": 'software activities'},
                                   {"name": 'classic music', "description": 'only classic music'}],
                    "metadata": {"count": 2}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminamos categoria 1
        self.CategoryDeleteSimple('software')

        # Chequeo de categorias
        response = self.getCategories()
        espected = {"categories": [{"name": 'classic music', "description": 'only classic music'}],
                    "metadata": {"count": 1}}
        self.assertEqual(json.dumps(espected), json.dumps(json.loads(response.text)))

        # Eliminamos categoria 3
        self.CategoryDeleteSimple('classic music')

        self.checkEmptyBD()

'''

def generate_user(username):
    return {
        "city" : "Rosario",
        "contacts" : [],
        "dob" : "1993-08-19",
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
        "recomendations" : [],
        "skills" :
            [
                "C",
                "C++",
                "GoogleTest"
            ],
        "email" : username
    }
if __name__ == '__main__':
    unittest.main()
