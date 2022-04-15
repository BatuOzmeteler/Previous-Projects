import numpy as np

from keras.layers import Input, Dense
from keras.models import Model
from keras.optimizers import Adam
from keras import backend as K

# Number of hidden size.
HIDDEN_SIZE = 128
# Number of layers.
NUM_LAYERS = 2
# Number of epochs.
EPOCHS = 3
# Learning rate. Lower lr stabilises training greatly.
LR = 1e-4
# Only implemented clipping for the surrogate loss, paper said it was best
LOSS_CLIPPING = 0.2
# Randomness in discrete action space.
ENTROPY_LOSS = 1e-3
# Batch size.
BATCH_SIZE = 64

class Actor:

    def __init__(self, num_state, num_actions):
        self.actor = self.build_actor(num_state, num_actions)
        # Dummy action and value to keep in this shape to neural network.
        self.DUMMY_ACTION, self.DUMMY_VALUE = np.zeros((1, num_actions)), np.zeros((1, 1))
        self.NUM_STATE = num_state
        self.NUM_ACTIONS = num_actions

    def proximal_policy_optimization_loss(self, advantage, old_prediction):
        def loss(y_true, y_pred):
            prob = y_true * y_pred
            old_prob = y_true * old_prediction
            r = prob/(old_prob + 1e-10)
            return -K.mean(K.minimum(r * advantage, K.clip(r, min_value=1 - LOSS_CLIPPING, max_value=1 + LOSS_CLIPPING) * advantage) + ENTROPY_LOSS * (prob * K.log(prob + 1e-10)))
        return loss

    def build_actor(self, num_state, num_actions):
        state_input = Input(shape=(num_state,))
        advantage = Input(shape=(1,))
        old_prediction = Input(shape=(num_actions,))

        x = Dense(HIDDEN_SIZE, activation='tanh')(state_input)
        for _ in range(NUM_LAYERS - 1):
            x = Dense(HIDDEN_SIZE, activation='tanh')(x)

        out_actions = Dense(num_actions, activation='softmax', name='output')(x)

        model = Model(inputs=[state_input, advantage, old_prediction], outputs=[out_actions])
        model.compile(optimizer=Adam(lr=LR),
                      loss=[self.proximal_policy_optimization_loss(
                          advantage=advantage,
                          old_prediction=old_prediction)])
        model.summary()

        return model

    def get_action(self, current_state):
        p = self.actor.predict([current_state.reshape(1, self.NUM_STATE), self.DUMMY_VALUE, self.DUMMY_ACTION])
        action = np.random.choice(self.NUM_ACTIONS, p=np.nan_to_num(p[0]))
        action_matrix = np.zeros(self.NUM_ACTIONS)
        action_matrix[action] = 1
        return action, action_matrix, p

    def fit(self, action, obs, advantage, old_prediction):
        actor_loss = self.actor.fit([obs, advantage, old_prediction], [action], batch_size=BATCH_SIZE, shuffle=True, epochs=EPOCHS, verbose=False)
        return actor_loss
